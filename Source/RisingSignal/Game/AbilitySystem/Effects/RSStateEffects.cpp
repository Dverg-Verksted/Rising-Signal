#include "RSStateEffects.h"

#include "RisingSignal/Game/AbilitySystem/ActorsComponents/RSHealthComponent.h"
#include "RisingSignal/Game/AbilitySystem/Interfaces/RSIGameActor.h"
#include "Kismet/GameplayStatics.h"

bool URSStateEffects::InitObject(AActor* Actor, FName NameBoneHit)
{
    EffectOwnerActor = Actor;

    IRSIGameActor* Interface = Cast<IRSIGameActor>(EffectOwnerActor);
    if (Interface)
    {
        Interface->AddEffect(this);
    }

    return true;
}

void URSStateEffects::DestroyObject()
{
    IRSIGameActor* Interface = Cast<IRSIGameActor>(EffectOwnerActor);
    if (Interface)
    {
        Interface->RemoveEffect(this);
    }

    EffectOwnerActor = nullptr;
    if (this && this->IsValidLowLevel())
    {
        this->ConditionalBeginDestroy();
    }
}

bool URSStateEffectExecuteOnce::InitObject(AActor* Actor, FName NameBoneHit)
{
    Super::InitObject(Actor, NameBoneHit);
    ExecuteOnce();
    return true;
}

void URSStateEffectExecuteOnce::DestroyObject()
{
    Super::DestroyObject();
}

void URSStateEffectExecuteOnce::ExecuteOnce()
{
    if (EffectOwnerActor)
    {
        URSHealthComponent* HealthComponent =
            Cast<URSHealthComponent>(EffectOwnerActor->GetComponentByClass(URSHealthComponent::StaticClass()));
        if (HealthComponent)
        {
            HealthComponent->ChangeHealthValue(Power);
        }
    }

    DestroyObject();
}

bool URSStateEffectExecuteTime::InitObject(AActor* Actor, FName NameBoneHit)
{
    Super::InitObject(Actor, NameBoneHit);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimer, this, &URSStateEffectExecuteTime::DestroyObject, Timer, false);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimer, this, &URSStateEffectExecuteTime::Execute, RateTime, true);

    if (ParticleEffect)
    {
        FName NameBoneToAttached = NameBoneHit;
        FVector Loc = FVector(0);

        USceneComponent* MeshImpactEffect =
            Cast<USceneComponent>(EffectOwnerActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
        if (MeshImpactEffect)
        {
            ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(
                ParticleEffect, MeshImpactEffect, NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
        }
        else
        {
            ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, EffectOwnerActor->GetRootComponent(),
                NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
        }
    }
    return true;
}

void URSStateEffectExecuteTime::DestroyObject()
{
    ParticleEmitter->DestroyComponent();
    ParticleEmitter = nullptr;
    Super::DestroyObject();
}

void URSStateEffectExecuteTime::Execute()
{
    if (EffectOwnerActor)
    {
        URSHealthComponent* HealthComponent =
            Cast<URSHealthComponent>(EffectOwnerActor->GetComponentByClass(URSHealthComponent::StaticClass()));
        if (HealthComponent)
        {
            HealthComponent->ChangeHealthValue(Power);
        }
    }
}
