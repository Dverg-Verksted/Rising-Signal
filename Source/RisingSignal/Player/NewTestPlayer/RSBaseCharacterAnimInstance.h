// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseCharacter.h"
#include "Animation/AnimInstance.h"
#include "Settings/AlsInAirSettings.h"
#include "RSBaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API URSBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slide animations")
    UAnimMontage* RollMontage;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    float Speed;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    float Direction;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    float LadderDirection;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    bool bIsFalling = false;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    bool bIsSprinting = false;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    bool bIsCrouching = false;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    bool bIsOnLadder = false;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float LowHeight = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float HighHeight = 50.0f;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    float FallHeight = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float LerpAlpha = 0.07f;

    UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category= "Character | IK System")
    FVector LeftFootEffectorLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category="Character | IK System")
    FVector RightFootEffectorLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category="Character | IK System")
    FVector PelvisOffset = FVector::ZeroVector;

private:

    void SetFallHeight(float NewValue);

    void OnRollStateChanged(bool State);

    TWeakObjectPtr<ARSBaseCharacter> CachedBaseCharacter;
};
