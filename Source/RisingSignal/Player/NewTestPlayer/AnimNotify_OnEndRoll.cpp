// It is owned by the company Dverg Verksted.


#include "Player/NewTestPlayer/AnimNotify_OnEndRoll.h"

#include "RSBaseCharacter.h"

void UAnimNotify_OnEndRoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    const ARSBaseCharacter* BaseCharacter = Cast<ARSBaseCharacter>(MeshComp->GetOwner());
    if(IsValid(BaseCharacter))
    {
        BaseCharacter->GetBaseCharacterMovementComponent()->StopRoll();
    }
}
