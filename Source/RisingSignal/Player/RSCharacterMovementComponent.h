// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "NewTestPlayer/RSBaseCharacter.h"
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
    CMOVE_Rolling UMETA(DisplayName = "Rolling")
};

UCLASS()
class RISINGSIGNAL_API URSCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    void StartSprint();
    void StopSprint();

    virtual float GetMaxSpeed() const override;

    void StartMantle(const FMantlingMovementParameters& MantlingMovementParameters);
    void EndMantle();

    void StartRoll();
    void StopRoll();

    FORCEINLINE float GetRollCapsuleHalfHeight() const { return RollCapsuleHalfHeight; }

protected:
    virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

    virtual void PhysCustom(float deltaTime, int32 Iterations) override;

    void PhysMantle(float DeltaTime, int32 Iterations);

    void PhysRolling(float DeltaTime, int32 Iterations);

    bool IsEnoughSpaceToStandUp();

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

private:
    TSoftObjectPtr<ARSBaseCharacter> BaseCharacterOwner;

    FMantlingMovementParameters CurrentMantlingParameters;
    FTimerHandle MantlingTimer;
    FTimerHandle RollingTimer;
    float RollDuration;
};
