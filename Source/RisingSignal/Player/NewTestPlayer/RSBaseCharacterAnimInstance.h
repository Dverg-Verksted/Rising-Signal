// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseCharacter.h"
#include "Animation/AnimInstance.h"
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

    void PlayRollMontage();

protected:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slide animations")
    UAnimMontage* RollMontage;

private:

    TWeakObjectPtr<ARSBaseCharacter> CachedBaseCharacter;
};
