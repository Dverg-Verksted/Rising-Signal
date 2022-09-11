// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacter.h"
#include "RSGamePlayerController.h"
#include "Game/AbilitySystem/BaseComponents/RSAbilitySystem.h"
#include "RSGamePLayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAlsCameraComponent;
class URSAbilitySystem;
class URSInventoryComponent;
class URSEquipmentComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventorySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJournalSignature);

/**
 * @class Own player (^_^)
 */
UCLASS(AutoExpandCategories = ("Settings|Als Character Example", "State|Als Character Example"))
class RISINGSIGNAL_API ARSGamePLayer : public AAlsCharacter
{
    GENERATED_BODY()

#pragma region Default

public:
    // Constructor
    ARSGamePLayer();

    // Called on start play simulate
    virtual void BeginPlay() override;

    /** 
    *	Function called every frame on this Actor. Override this function to implement custom logic to be executed every frame.
    *	Note that Tick is disabled by default, and you will need to check PrimaryActorTick.bCanEverTick is set to true to enable it.
    *
    *	@param	DeltaSeconds	Game time elapsed during last frame modified by the time dilation
    */
    virtual void Tick(float DeltaSeconds) override;

    virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& ViewInfo) override;

#pragma endregion

#pragma region Components

public:
    UFUNCTION(BlueprintGetter)
    URSInventoryComponent* GetInventoryComponent();

    UFUNCTION(BlueprintGetter)
    URSEquipmentComponent* GetEquipmentComponent();
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    URSAbilitySystem* AbilitySystem;

private:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    UCameraComponent* Camera;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    URSInventoryComponent* InventoryComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    URSEquipmentComponent* EquipmentComponent;


#pragma endregion

#pragma region PlayerInput

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character | PlayerInput",
        Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
    float LookUpMouseSensitivity{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character | PlayerInput",
        Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
    float LookRightMouseSensitivity{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character | PlayerInput",
        Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "deg"))
    float LookUpRate{45.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character | PlayerInput",
        Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "deg"))
    float LookRightRate{90.0f};

protected:
    /** Allows a Pawn to set up custom input bindings. Called upon possession by a PlayerController, using the InputComponent created by CreatePlayerInputComponent(). */
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    FTimerHandle SprintStartTimer;

    void InputLookUp(float Value);

    void InputLookRight(float Value);

    void InputMoveForward(float Value);

    void InputMoveRight(float Value);

    void InputSprintPressed();

    void InputSprintReleased();

    void InputRoll();

    void InputWalk();

    void InputCrouch();

    void InputJumpPressed();

    void InputJumpReleased();

    void InputRotationModePressed();

    void InputRagdollPressed();

    void InputViewModePressed();

    void InputSwitchShoulderPressed();

    void InputActionSlot1();

    void InputActionSlot2();

    void InputActionSlot3();

    void InputActionSlot4();

#pragma endregion

#pragma region Debug

public:
    virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused, float& VerticalPosition) override;

#pragma endregion

#pragma region Extension

public:
    
    UPROPERTY(BlueprintReadOnly)
    ARSGamePlayerController* GamePlayerController;

    UPROPERTY(BlueprintAssignable)
    FOnInventorySignature InventoryOpenClose;

    UPROPERTY(BlueprintAssignable)
    FOnJournalSignature JournalOpenClose;

private:

    bool canRun = true;

    void OpenCloseInventory();

    void OpenCloseJournal();
    
    UFUNCTION()
    void CheckSomeState(EAbilityStatesType StateTyp, float Value);
    
    UFUNCTION()
    void RegisterDeath();

#pragma endregion Extension
};
