// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EndMantle.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API UAnimNotify_EndMantle : public UAnimNotify
{
	GENERATED_BODY()

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
