// It is owned by the company Dverg Verksted.

#include "RSGameMode.h"

#include "Game/SaveLoad/RSSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RSGamePlayerController.h"
#include "Player/RSPlayerState.h"


void ARSGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{

    // (Save/Load logic moved into new SaveGameSubsystem)
    URSSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>();

    // Optional slot name (Falls back to slot specified in SaveGameSettings class/INI otherwise)
    FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
    SG->LoadSaveGame(SelectedSaveSlot);
    Super::InitGame(MapName, Options, ErrorMessage);
}

void ARSGameMode::OnChecpointReached(AActor* ReachedActor)
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
    // Calling Before Super:: so we set variables before 'beginplayingstate' is called in PlayerController (which is where we instantiate UI)
    URSSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>();
    SG->HandleStartingNewPlayer(NewPlayer);

    Super::HandleStartingNewPlayer_Implementation(NewPlayer);

    // Now we're ready to override spawn location
    // Alternatively we could override core spawn location to use store locations immediately (skipping the whole 'find player start' logic)
    SG->OverrideSpawnTransform(NewPlayer);
}

void ARSGameMode::BeginPlay()
{
    Super::BeginPlay();

    this->PC = Cast<ARSGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    check(this->PC);

    this->GameHUD = Cast<AGameHUD>(this->PC->GetHUD());
    check(this->GameHUD);

}
