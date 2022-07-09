// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "RSGamePlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractSignature);

/**
 *
 */
UCLASS()
class RISINGSIGNAL_API ARSGamePlayerController : public APlayerController
{
    GENERATED_BODY()

#pragma region Default

    ARSGamePlayerController();

    virtual void SetupInputComponent() override;

#pragma endregion

#pragma region Signature

public:

    UPROPERTY(BlueprintAssignable, Category = "ARSGamePlayerController | Signature")
    FOnInteractSignature OnInteract;

#pragma endregion

#pragma region PlayerInput

    UFUNCTION()
    void InputActionInteract();

#pragma endregion 
};
