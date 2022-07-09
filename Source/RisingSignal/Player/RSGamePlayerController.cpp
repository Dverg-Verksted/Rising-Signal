// It is owned by the company Dverg Verksted.

#include "Player/RSGamePlayerController.h"

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
    }
}

void ARSGamePlayerController::InputActionInteract()
{
    OnInteract.Broadcast();
}

#pragma endregion
