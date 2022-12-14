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

}

void ARSGameMode::CheckpointReached(AActor* ReachedActor)
{
    ARSBaseCharacter* Player = Cast<ARSBaseCharacter>(ReachedActor);
    if (Player)
    {
        URSSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>();
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

    URSSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>();
    SG->HandleStartingNewPlayer(Controller);
    SG->OverrideSpawnTransform(Controller);

    // Optional slot name (Falls back to slot specified in SaveGameSettings class/INI otherwise)
    FString SelectedSaveSlot = UGameplayStatics::ParseOption(OptionsString, "SaveGame");
    SG->LoadSaveGame(SelectedSaveSlot);
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

    // FString SelectedSaveSlot = UGameplayStatics::ParseOption(OptionsString, "SaveGame");

    // UGameplayStatics::DeleteGameInSlot(SelectedSaveSlot, 0);

    // OptionsString+="";
    // UKismetSystemLibrary::ExecuteConsoleCommand(this, "RestartLevel");
}

void ARSGameMode::BeginPlay()
{
    Super::BeginPlay();

    this->PC = Cast<ARSGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    check(this->PC);

    this->GameHUD = Cast<AGameHUD>(this->PC->GetHUD());
    check(this->GameHUD);

    URSSaveGameSubsystem* SGS = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>();

    if (!UGameplayStatics::DoesSaveGameExist(SGS->GetSlotName(), 0))
    {
        SGS->SetSaveGame(Cast<URSSaveGame>(UGameplayStatics::CreateSaveGameObject(URSSaveGame::StaticClass())));

        UE_LOG(LogTemp, Warning, TEXT("Created New SaveGame Data. 1111111"));
    }
    else
    {
        SGS->SetSaveGame(Cast<URSSaveGame>(UGameplayStatics::LoadGameFromSlot(SGS->GetSlotName(), 0)));
        if (SGS->GetSaveGame() == nullptr)
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
