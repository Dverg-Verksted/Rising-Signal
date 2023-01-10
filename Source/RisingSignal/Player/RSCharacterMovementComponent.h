// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/InteractSystem/Environment/Ladder/Ladder.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RSCharacterMovementComponent.generated.h"

class ARSBaseCharacter;

struct FMantlingMovementParameters
{
    FVector InitialLocation = FVector::ZeroVector;
    FRotator InitialRotation = FRotator::ZeroRotator;

    FVector TargetLocation = FVector::ZeroVector;
    FRotator TargetRotation = FRotator::ZeroRotator;
    FVector InitialAnimationLocation = FVector::ZeroVector;

    float Duration = 1.0f;
    float StartTime = 0.0f;

    UCurveVector* MantlingCurve;

    UPrimitiveComponent* PrimitiveComponent;
    FVector PrimitiveComponentInitialLocation;
};

UENUM(BlueprintType)
enum class EDetachFromLadderMethod : uint8
{
    Fall = 0,
    ReachingTheTop,
    ReachingTheBottom,
    JumpOff
};

USTRUCT(BlueprintType)
struct FRollingMovementaParamaters
{
    GENERATED_BODY()
    
    float Duration = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UCurveFloat* RollingCurve;
};

UENUM()
enum class ECustomMovementMode : uint8
{
    CMOVE_None = 0 UMETA(DisplayName = "None"),
    CMOVE_Mantling UMETA(DisplayName = "Mantling"),
    CMOVE_Rolling UMETA(DisplayName = "Rolling"),
    CMOVE_AttachingOnLadder UMETA(DisplayName = "AttachingOnLadder"),
    CMOVE_AttachingOnTopLadder UMETA(DisplayName = "AttachingOnTopLadder"),
    CMOVE_OnLadder UMETA(DisplayName = "Ladder"),
    CMOVE_OnRope UMETA(DisplayName = "Rope")
};

UCLASS()
class RISINGSIGNAL_API URSCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    virtual float GetMaxSpeed() const override;

    void StartMantle(const FMantlingMovementParameters& MantlingMovementParameters);
    void EndMantle();

    void StartRoll();
    void StopRoll();

    void AttachToLadder(const ALadder* Ladder);
    void AttachToLadderFromTop();
    void DetachFromLadder(EDetachFromLadderMethod DetachFromLadderMethod);
    void SetLadderMovement();
    void SetWalkingMovement();
    bool IsOnLadder() const;
    const ALadder* GetCurrentLadder() const { return CurrentLadder; }

    FORCEINLINE float GetRollCapsuleHalfHeight() const { return RollCapsuleHalfHeight; }

    bool bIsAttachingToLadder = false;

protected:
    virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

    virtual void PhysCustom(float deltaTime, int32 Iterations) override;

    void PhysMantle(float DeltaTime, int32 Iterations);

    void PhysRolling(float DeltaTime, int32 Iterations);

    void PhysAttachToLadder(float DeltaTime, int32 Iterations);

    void PhysAttachOnTopLadder(float DeltaTime, int32 Iterations);

    void PhysLadder(float DeltaTime, int32 Iterations);

    void PhysHanging(float DeltaTime, int32 Iterations);

    bool IsEnoughSpaceToStandUp();

    float GetActorToCurrentLadderProjection(FVector ActorLocation);

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRollingMovementaParamaters CurrentRollingParameters;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Roll parameters")
    float RollCapsuleHalfHeight = 40.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Speed parameters")
    float RollSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Speed parameters")
    float SprintSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Speed parameters")
    float CrouchSpeed = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character movement: ladder", meta=(ClampMin = 0.0f, UIMin = 0.0f));
    float ClimbingOnLadderSpeed = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character movement: ladder", meta=(ClampMin = 0.0f, UIMin = 0.0f));
    float LadderToCharacterOffset = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character movement: ladder", meta=(ClampMin = 0.0f, UIMin = 0.0f));
    float ClimbingOnLadderBrakingDecelaration = 2048.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character movement: ladder", meta=(ClampMin = 0.0f, UIMin = 0.0f));
    float MinLadderBotomOffset = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character movement: ladder", meta=(ClampMin = 0.0f, UIMin = 0.0f));
    float MaxLadderTopOffset = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character movement: ladder", meta=(ClampMin = 0.0f, UIMin = 0.0f));
    float JumpOffFromLadderSpeed = 500.0f;

private:
    TSoftObjectPtr<ARSBaseCharacter> BaseCharacterOwner;

    UPROPERTY()
    const ALadder* CurrentLadder = nullptr;

    FMantlingMovementParameters CurrentMantlingParameters;
    FTimerHandle MantlingTimer;
    FTimerHandle RollingTimer;
    FTimerHandle LadderTimer;
    float RollDuration;
};
