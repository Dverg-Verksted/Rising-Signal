// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/Environment/InteractiveActor.h"

#include "Components/CapsuleComponent.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"


void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();

    if(IsValid(InteractionVolume))
    {
        InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveActor::OnInteractionVolumeStartOverlap);
        InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AInteractiveActor::OnInteractionVolumeEndOverlap);
    }
}

bool AInteractiveActor::IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp) const
{
    ARSBaseCharacter* BaseCharacter = Cast<ARSBaseCharacter>(OtherActor);
    if(!IsValid(BaseCharacter))
    {
        return false;
    }

    if(Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
    {
        return false;
    }
    return true;
}

void AInteractiveActor::OnInteractionVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
    {
        return;
    }

    ARSBaseCharacter* BaseCharacter = Cast<ARSBaseCharacter>(OtherActor);
    BaseCharacter->RegisterInteractiveActor(this);
}

void AInteractiveActor::OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
    {
        return;
    }

    ARSBaseCharacter* BaseCharacter = Cast<ARSBaseCharacter>(OtherActor);
    BaseCharacter->UnRegisterInteractiveActor(this);
}


