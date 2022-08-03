// It is owned by the company Dverg Verksted.


#include "Game/AI/Animations/RSBiteAnimNotify.h"

void URSBiteAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComp);

    Super::Notify(MeshComp, Animation);
}
