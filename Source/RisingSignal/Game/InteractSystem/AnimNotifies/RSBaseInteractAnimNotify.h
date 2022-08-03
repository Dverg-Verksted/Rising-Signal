// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RSBaseInteractAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNotifiedSignature_Interact);


/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API URSBaseInteractAnimNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    FOnNotifiedSignature_Interact OnNotifiedInteract;
};
