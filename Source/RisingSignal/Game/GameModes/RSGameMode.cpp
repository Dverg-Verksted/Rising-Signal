// It is owned by the company Dverg Verksted.

#include "RSGameMode.h"

#include "Game/SaveLoad/RSSaveGame.h"
#include "Game/SaveLoad/RSSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/RSGamePlayerController.h"
#include "Player/RSPlayerState.h"


void ARSGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{

    Super::InitGame(MapName, Options, ErrorMessage);

    GetWorld()->OnWorldBeginPlay.AddUObject(this, &ARSGameMode::GameStarted);

}

void ARSGameMode::StartPlay()
{
    UE_LOG(LogTemp, Warning, TEXT("%s::%s() called"), *GetName(), *FString(__FUNCTION__));

    Super::StartPlay();

    if (bGameWasLoaded)
    {
        SG->OnSaveGameLoaded.Broadcast(SG->GetSaveGame());
    }

}

void ARSGameMode::CheckpointReached(AActor* ReachedActor)
{
    ARSBaseCharacter* Player = Cast<ARSBaseCharacter>(ReachedActor);
    if (Player && SG)
    {
        // Immediately auto save on death
        SG->WriteSaveGame();
    }
}

void ARSGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ARSGameMode::LoadCheckpoint()
{
    ARSGamePlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSGamePlayerController>();

    if (!SG)
    {
        LOG_RS(ELogRSVerb::Error, "No SaveGame Subsystem");
    }

    SG->HandleStartingNewPlayer(Controller);
    SG->OverrideSpawnTransform(Controller);
    SG->OnPlayerLoaded.Broadcast(SG->GetSaveGame());

    // Optional slot name (Falls back to slot specified in SaveGameSettings class/INI otherwise)
    FString SelectedSaveSlot = UGameplayStatics::ParseOption(OptionsString, "SaveGame");

    bGameWasLoaded = SG->LoadSaveGame(SelectedSaveSlot);

}

void ARSGameMode::LoadLevel(FName LevelName, bool FromBegin)
{
    if (FromBegin)
        UGameplayStatics::OpenLevel(this, LevelName, true, L"LoadCheckpoint=False");
    else
        UGameplayStatics::OpenLevel(this, LevelName, true, L"LoadCheckpoint=True");
}

void ARSGameMode::RestartLevel()
{
}

void ARSGameMode::BeginPlay()
{
    UE_LOG(LogTemp, Warning, TEXT("%s::%s() called"), *GetName(), *FString(__FUNCTION__));
    Super::BeginPlay();

    this->PC = Cast<ARSGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    check(this->PC);

    this->GameHUD = Cast<AGameHUD>(this->PC->GetHUD());
    check(this->GameHUD);

    SG = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>();

    if (!UGameplayStatics::DoesSaveGameExist(SG->GetSlotName(), 0))
    {
        SG->SetSaveGame(Cast<URSSaveGame>(UGameplayStatics::CreateSaveGameObject(URSSaveGame::StaticClass())));

        UE_LOG(LogTemp, Warning, TEXT("Created New SaveGame Data. 1111111"));
    }
    else
    {
        SG->SetSaveGame(Cast<URSSaveGame>(UGameplayStatics::LoadGameFromSlot(SG->GetSlotName(), 0)));
        if (SG->GetSaveGame() == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
            return;
        }
    }

    if (UGameplayStatics::ParseOption(OptionsString, "LoadCheckpoint") == "True")
    {
        UE_LOG(LogTemp, Warning, TEXT("Checkpoint"));
        LoadCheckpoint();
    }

}

void ARSGameMode::GameStarted()
{
    // if (bGameWasLoaded)
    // {
    //     SG->OnSaveGameLoaded.Broadcast(SG->GetSaveGame());
    // }
}
