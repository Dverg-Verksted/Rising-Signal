// It is owned by the company Dverg Verksted.


#include "Game/SaveLoad/RSSaveGameSubsystem.h"

#include "EngineUtils.h"
#include "RSSavableObjectInterface.h"
#include "RSSaveGame.h"
#include "RSSaveGameSettings.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/RSPlayerState.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


void URSSaveGameSubsystem::HandleStartingNewPlayer(AController* NewPlayer)
{
    ARSPlayerState* PS = NewPlayer->GetPlayerState<ARSPlayerState>();
    if (ensure(PS))
    {
        PS->LoadPlayerState(CurrentSaveGame);
    }
}

bool URSSaveGameSubsystem::OverrideSpawnTransform(AController* NewPlayer)
{
    if (!IsValid(NewPlayer))
    {
        return false;
    }

    APlayerState* PS = NewPlayer->GetPlayerState<APlayerState>();
    if (PS == nullptr)
    {
        return false;
    }

    if (APawn* MyPawn = PS->GetPawn())
    {
        FPlayerSaveData* FoundData = CurrentSaveGame->GetPlayerData();
        if (FoundData && FoundData->bResumeAtTransform)
        {
            MyPawn->SetActorLocation(FoundData->Location);
            MyPawn->SetActorRotation(FoundData->Rotation);

            return true;
        }
    }

    return false;
}

void URSSaveGameSubsystem::SetSlotName(FString NewSlotName)
{
    // Ignore empty name
    if (NewSlotName.Len() == 0)
    {
        return;
    }

    CurrentSlotName = NewSlotName;
}

void URSSaveGameSubsystem::WriteSaveGame()
{

    UE_LOG(LogTemp, Warning, TEXT("%s::%s() called"), *GetName(), *FString(__FUNCTION__));

    if (!CurrentSaveGame)
    {
        LOG_RS(ELogRSVerb::Error, "No SaveGame Found!");
        return;
    }

    // Clear arrays, may contain data from previously loaded SaveGame
    CurrentSaveGame->ClearData();

    AGameStateBase* GS = GetWorld()->GetGameState();
    if (GS == nullptr)
    {
        // Warn about failure to save?
        return;
    }

    // Iterate all player states, we don't have proper ID to match yet (requires Steam or EOS)
    for (int32 i = 0; i < GS->PlayerArray.Num(); i++)
    {
        ARSPlayerState* PS = Cast<ARSPlayerState>(GS->PlayerArray[i]);
        if (PS)
        {
            PS->SavePlayerState(CurrentSaveGame);
            break; // single player only at this point
        }
    }

    // Iterate the entire world of actors
    for (FActorIterator It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        // Only interested in our 'gameplay actors', skip actors that are being destroyed
        // Note: You might instead use a dedicated SavableObject interface for Actors you want to save instead of re-using GameplayInterface
        if (!IsValid(Actor) || !Actor->Implements<URSSavableObjectInterface>())
        {
            continue;
        }

        FActorSaveData ActorData;
        ActorData.ActorName = Actor->GetFName();
        ActorData.Transform = Actor->GetActorTransform();

        // Pass the array to fill with data from Actor
        FMemoryWriter MemWriter(ActorData.ByteData);

        FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
        // Find only variables with UPROPERTY(SaveGame)
        Ar.ArIsSaveGame = true;
        // Converts Actor's SaveGame UPROPERTIES into binary array
        Actor->Serialize(Ar);

        CurrentSaveGame->SavedActors.Add(ActorData);
    }

    UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSlotName, 0);

    OnSaveGameWritten.Broadcast(CurrentSaveGame);
}


void URSSaveGameSubsystem::LoadSaveGame(FString InSlotName)
{
    // Update slot name first if specified, otherwise keeps default name
    SetSlotName(InSlotName);

    if (UGameplayStatics::DoesSaveGameExist(CurrentSlotName, 0))
    {
        CurrentSaveGame = Cast<URSSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));
        if (CurrentSaveGame == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
            return;
        }

        UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame Data."));

        

        // Iterate the entire world of actors
        for (FActorIterator It(GetWorld()); It; ++It)
        {
            AActor* Actor = *It;
            // Only interested in our 'gameplay actors'
            if (!Actor->Implements<URSSavableObjectInterface>())
            {
                continue;
            }

            for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
            {
                if (ActorData.ActorName == Actor->GetFName())
                {
                    Actor->SetActorTransform(ActorData.Transform);

                    FMemoryReader MemReader(ActorData.ByteData);

                    FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
                    Ar.ArIsSaveGame = true;
                    // Convert binary array back into actor's variables
                    Actor->Serialize(Ar);

                    IRSSavableObjectInterface::Execute_OnActorLoaded(Actor);

                    break;
                }
            }
        }

        OnSaveGameLoaded.Broadcast(CurrentSaveGame);
    }
    else
    {
        CurrentSaveGame = Cast<URSSaveGame>(UGameplayStatics::CreateSaveGameObject(URSSaveGame::StaticClass()));

        UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
    }
}


void URSSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    const URSSaveGameSettings* SGSettings = GetDefault<URSSaveGameSettings>();
    // Access defaults from DefaultGame.ini
    CurrentSlotName = SGSettings->SaveSlotName;

    // Make sure it's loaded into memory .Get() only resolves if already loaded previously elsewhere in code
    UDataTable* DummyTable = SGSettings->DummyTablePath.LoadSynchronous();
    //DummyTable->GetAllRows() // We don't need this table for anything, just an content reference example
}

void URSSaveGameSubsystem::SetSaveGame(URSSaveGame* SaveGame)
{
    CurrentSaveGame = SaveGame;
}
