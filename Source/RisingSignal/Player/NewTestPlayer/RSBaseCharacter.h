// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "RSLedgeDetectorComponent.h"
#include "Game/AbilitySystem/BaseComponents/RSAbilitySystem.h"
#include "Game/InteractSystem/Environment/InteractiveActor.h"
#include "Game/SaveLoad/RSSavableObjectInterface.h"
#include "GameFramework/Character.h"
#include "Player/RSCharacterMovementComponent.h"
#include "RSBaseCharacter.generated.h"

class ALadder;
class ARope;
class ARSGamePlayerController;
class UCameraComponent;
class UWeaponComponent;
class URSCraftComponent;
class URSEquipmentComponent;
class URSInventoryComponent;
class USpringArmComponent;

enum EEquipSlots
{
    EquipSlot1,
    EquipSlot2,
    EquipSlot3,
    EquipSlot4
};

USTRUCT(BlueprintType)
struct FMantlingSettings
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    class UAnimMontage* MantlingMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    class UCurveVector* MantlingCurve;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float AnimationCorrectionXY = 65.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float AnimationCorrectionZ = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta= (ClampMin = 0.0f, UIMin = 0.0f))
    float MaxHeight = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
    float MinHeight = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta= (ClampMin = 0.0f, UIMin = 0.0f))
    float MaxHeightStartTime = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
    float MinHeightStartTime = 0.5f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventorySignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJournalSignature);

DECLARE_DELEGATE(FOnSlideSignature);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLandedSignature, float);

DECLARE_DELEGATE_OneParam(FOnRollStateChangedSignature, bool);

UCLASS()
class RISINGSIGNAL_API ARSBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ARSBaseCharacter(const FObjectInitializer& ObjectInitializer);

    FOnSlideSignature OnSlide;
    FOnLandedSignature OnLanded;
    FOnRollStateChangedSignature OnRollStateChangedSignature;

    virtual void Falling() override;
    virtual void Landed(const FHitResult& Hit) override;
    virtual void NotifyJumpApex() override;
    
    virtual void Jump() override;

    FORCEINLINE URSCharacterMovementComponent* GetBaseCharacterMovementComponent() const { return RSCharacterMovementComponent; }
    FORCEINLINE URSInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
    FORCEINLINE UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE URSAbilitySystem* GetAbilitySystem() const { return AbilitySystem; }

    UFUNCTION(BlueprintPure)
    FORCEINLINE USkeletalMeshComponent* GetExtraMesh() const { return ExtraSkeletalMesh; }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE bool IsHanging() const { return bIsHanging; }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE bool IsClimbingOnWall() const { return bIsClimbingOnWall; }

    UFUNCTION(BlueprintCallable)
    void SetIsHanging(bool NewValue) { bIsHanging = NewValue; }

    UFUNCTION(BlueprintCallable)
    void SetIsClimbingOnWall(bool NewValue) { bIsClimbingOnWall = NewValue; }

    void Mantle(bool bForce = false);
    void Roll();

    FORCEINLINE bool GetIsMantling() const;
    void SetIsMantling(bool NewValue);

    FORCEINLINE bool GetIsRolling() const;
    void SetIsRolling(bool NewValue);

    FORCEINLINE bool GetIsSprinting() const;

    void OnStartRoll(float HalfHeightAdjust);
    void OnStopRoll(float HalfHeightAdjust);

    void ClimbLadder(float Value);
    void SwingRope(float Value);
    
    void MoveWallForward(float Value);
    
    void MoveWallRight(float Value);

    void RegisterInteractiveActor(AInteractiveActor* InteractiveActor);
    void UnRegisterInteractiveActor(AInteractiveActor* InteractiveActor);

    void InteractWithLadder();
    void InteractWithRope(ARope* Rope);
    void InteractWithWall();

    void DetachFromWall();

    UPROPERTY()
    float CurrentHeight = 0.0f;

    void UpdateCameraRotation();

    const ALadder* GetAvailableLadder() const;
    ARope* GetAvailableRope() const;
    AClimbingWall* GetAvailableWall() const;
    
    UFUNCTION(BlueprintCallable, BlueprintPure)
    FORCEINLINE float GetIKLeftFootOffset() const {return IKLeftFootOffset;}

    UFUNCTION(BlueprintCallable, BlueprintPure)
    FORCEINLINE float GetIKRightFootOffset() const {return IKRightFootOffset;}

    UFUNCTION(BlueprintCallable, BlueprintPure)
    FORCEINLINE float GetIKPelvisOffset() const {return IKPelvisOffset;}

    FORCEINLINE float GetHangingSpeed() const { return HangingSpeed; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    TArray<AInteractiveActor*> AvailableInteractiveActors;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Landed settings", DisplayName="Минимальная высота для кувырка",
    meta=(ToolTip="Выше этой высоты персонаж будет делать кувырок при приземлении. Должна быть меньше максимальной высоты для кувырка", ClampMin = 0.0f, UIMin = 0.0f))
    float MinHeightForRoll = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Landed settings", DisplayName="Максимальная высота для кувырка",
    meta=(ToolTip="Выше этой высоты персонаж не сможет сделать кувырок при приземлении. Должна быть не меньше минимальной высоты для кувырка", ClampMin = 0.0f, UIMin = 0.0f))
    float MaxHeightForRoll = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Falls")
    UCurveFloat* FallDamageCurve;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character | IK System")
    FName IKLeftFootSocketName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character | IK System")
    FName IKRightFootSocketName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character | IK System")
    float IKInterpSpeed = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character | IK System")
    float IKExtendedDistance = 60.0f;

#pragma region Components

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    URSCharacterMovementComponent* RSCharacterMovementComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    URSAbilitySystem* AbilitySystem;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    UCameraComponent* Camera;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    URSInventoryComponent* InventoryComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    URSEquipmentComponent* EquipmentComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    URSCraftComponent* CraftComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    UWeaponComponent* WeaponComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    URSLedgeDetectorComponent* LedgeDetectorComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
    USkeletalMeshComponent* ExtraSkeletalMesh;

#pragma endregion

#pragma region MantlingSettings

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mantling")
    FMantlingSettings HighMantlingSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mantling")
    FMantlingSettings LowMantlingSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mantling")
    float LowMantleMaxHeight = 125.0f;

#pragma endregion

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Input

private:
    void InputLookUp(float Value);

    void InputLookRight(float Value);

    void InputMoveForward(float Value);

    void InputMoveRight(float Value);

    void InputLadder(float Value);

    void InputRope(float Value);

    void InputWallForward(float Value);

    void InputWallRight(float Value);

    void InputSprintPressed();

    void InputSprintReleased();

    void InputRoll();

    void InputWalk();

    void InputCrouch();

    void InputInteractLadder();

    void InputJumpPressed();

    void InputJumpReleased();

    void InputRagdollPressed();

    void InputActionSlot1();

    void InputActionSlot2();

    void InputActionSlot3();

    void InputActionSlot4();

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

#pragma endregion

#pragma region Extension

public:
    UPROPERTY(BlueprintReadOnly)
    ARSGamePlayerController* GamePlayerController;

    UPROPERTY(BlueprintAssignable)
    FOnInventorySignature InventoryOpenClose;

    UPROPERTY(BlueprintAssignable)
    FOnJournalSignature JournalOpenClose;

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
        AActor* DamageCauser) override;

private:
    bool canRun = true;

    /**
     * @brief Open inventory in game HUD
     */
    void OpenCloseInventory();

    /**
     * @brief Open Journal in game HUD
     */
    void OpenCloseJournal();

    /**
     * @brief Call on ability system states changes
     * @param StateTyp - Type of changed ability state
     * @param Value - Value which param changed
     */
    UFUNCTION()
    void CheckSomeState(EAbilityStatesType StateTyp, float Value);

    UFUNCTION()
    void RegisterDeath();

#pragma endregion Extension

private:
    const FMantlingSettings& GetMantlingSettings(float LedgeHeight) const;

    void GetControlRightVector(FVector& Right);
    
    bool CanMove();
    bool CanMantle();
    bool CanRoll();

    float GetIKFootOffset(const FName& SocketName);
    float GetPelvisOffset();
    void CalculateOffsets(float DeltaSeconds);

    float HangingSpeed = 0.0f;

    float IKLeftFootOffset = 0.0f;
    float IKRightFootOffset = 0.0f;
    float IKPelvisOffset = 0.0f;
    float IKTraceDistance = 50.0f;
    float IKScale = 0.0f;
    float CapsuleRadius = 0.0f;
    float MinimumOffset = 3.0f;

    bool bIsMantling;
    bool bIsRolling;
    bool bIsSprinting;
    bool bIsHanging;
    bool bIsClimbingOnWall;
};
