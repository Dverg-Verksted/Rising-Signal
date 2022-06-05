#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Particles/ParticleSystemComponent.h"
#include "RSStateEffects.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API URSStateEffects : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * @brief 
	 */
	AActor* MyActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<TEnumAsByte<EPhysicalSurface>> PossibleInteractSurface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Particle")
	UParticleSystem* ParticleEffect = nullptr;

	/**
	 * @brief 
	 */
	UParticleSystemComponent* ParticleEmitter = nullptr;

	virtual bool InitObject(AActor* Actor, FName NameBoneHit);
	virtual void DestroyObject();
};

UCLASS()
class RISINGSIGNAL_API URSStateEffectExecuteOnce : public URSStateEffects
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float Power = 20.0f;

	bool InitObject(AActor* Actor, FName NameBoneHit) override;
	void DestroyObject() override;

	virtual void ExecuteOnce();
};

UCLASS()
class RISINGSIGNAL_API URSStateEffectExecuteTime : public URSStateEffects
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting ExecuteTimer")
	float Power = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting ExecuteTimer")
	float Timer = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting ExecuteTimer")
	float RateTime = 1.0f;

	FTimerHandle TimerHandle_ExecuteTimer;
	FTimerHandle TimerHandle_EffectTimer;

	bool InitObject(AActor* Actor, FName NameBoneHit) override;
	void DestroyObject() override;

	virtual void Execute();
};
