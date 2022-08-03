// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/AnimNotifies/RSBaseInteractAnimNotify.h"

void URSBaseInteractAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotifiedInteract.Broadcast();

    Super::Notify(MeshComp, Animation);
}
