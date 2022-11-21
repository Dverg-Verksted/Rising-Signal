// It is owned by the company Dverg Verksted.


#include "Player/NewTestPlayer/AnimNotify_EndMantle.h"

#include "RSBaseCharacter.h"

void UAnimNotify_EndMantle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    ARSBaseCharacter* PlayerCharacter = Cast<ARSBaseCharacter>(MeshComp->GetOwner());
    if(IsValid(PlayerCharacter))
    {
        PlayerCharacter->SetIsMantling(false);
    }
}
