// It is owned by the company Dverg Verksted.

#include "Player/RSGamePlayerController.h"

#include "GameFramework/GameModeBase.h"

#pragma region Default

ARSGamePlayerController::ARSGamePlayerController()
{
}

void ARSGamePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (InputComponent)
    {
        InputComponent->BindAction(FName("Interact"), EInputEvent::IE_Pressed, this, &ThisClass::InputActionInteract);
        InputComponent->BindAction("Pause", IE_Pressed, this, &ARSGamePlayerController::PauseGame);
    }
}

void ARSGamePlayerController::InputActionInteract()
{
    OnInteract.Broadcast();
}

void ARSGamePlayerController::PauseGame()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    if (GetWorld()->GetAuthGameMode()->SetPause(this))
    {
        OnGamePaused.Broadcast();
    }
}

#pragma endregion
