// It is owned by the company Dverg Verksted.


#include "Game/AI/Cephalopod/RSAICephalopod.h"

ARSAICephalopod::ARSAICephalopod()
{
}

void ARSAICephalopod::BeginPlay()
{
    Super::BeginPlay();
}

void ARSAICephalopod::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ARSAICephalopod::AIStateChanged(EAIState NewState, EAIState PrevState)
{
    Super::AIStateChanged(NewState, PrevState);
}

void ARSAICephalopod::Attack(AActor* ActorToAttack)
{
    Super::Attack(ActorToAttack);
}

void ARSAICephalopod::ProvideDamage(USkeletalMeshComponent* FromMeshComponent)
{
    Super::ProvideDamage(FromMeshComponent);
}
