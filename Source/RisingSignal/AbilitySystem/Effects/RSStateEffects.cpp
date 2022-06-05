#include "RSStateEffects.h"

#include "RisingSignal/AbilitySystem/ActorsComponents/RSHealthComponent.h"
#include "RisingSignal/AbilitySystem/Interfaces/RSIGameActor.h"
#include "Kismet/GameplayStatics.h"

bool URSStateEffects::InitObject(AActor* Actor, FName NameBoneHit)
{
	MyActor = Actor;

	IRSIGameActor* myInterface = Cast<IRSIGameActor>(MyActor);
	if (myInterface)
	{
		myInterface->AddEffect(this);
	}

	return true;
}

void URSStateEffects::DestroyObject()
{
	IRSIGameActor* myInterface = Cast<IRSIGameActor>(MyActor);
	if (myInterface)
	{
		myInterface->RemoveEffect(this);
	}

	MyActor = nullptr;
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
	if (MyActor)
	{
		URSHealthComponent* MyHealthComponent = Cast<URSHealthComponent>(MyActor->GetComponentByClass(URSHealthComponent::StaticClass()));
		if (MyHealthComponent)
		{
			MyHealthComponent->ChangeHealthValue(Power);
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

		USceneComponent* MyMesh = Cast<USceneComponent>(MyActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		if (MyMesh)
		{
			ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect,
				MyMesh, NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
		}
		else
		{
			ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect,
				MyActor->GetRootComponent(), NameBoneToAttached, Loc,
				FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
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
	if (MyActor)
	{
		URSHealthComponent* MyHealthComponent = Cast<URSHealthComponent>(MyActor->GetComponentByClass(URSHealthComponent::StaticClass()));
		if (MyHealthComponent)
		{
			MyHealthComponent->ChangeHealthValue(Power);
		}
	}
}
