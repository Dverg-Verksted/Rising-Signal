#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Particles/ParticleSystemComponent.h"
#include "RSStateEffects.generated.h"

/**
 * The base class for effects.,
 */
UCLASS()
class RISINGSIGNAL_API URSStateEffects : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * @brief Actor witch will have effect.
	 */
	AActor* EffectOwnerActor = nullptr;

	/*A list of all the possible surfaces that the effect can interact with.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<TEnumAsByte<EPhysicalSurface>> PossibleInteractSurface;

	/*Value witch show effect can be stackable effects.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	bool bIsStackable = false;

	/*A variable that can be edited in the editor.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Particle")
	UParticleSystem* ParticleEffect = nullptr;

	/**
	 * @brief for particle effects emitter.
	 */
	UParticleSystemComponent* ParticleEmitter = nullptr;

	/**
	 * This function is called when the object is created. It sets the actor that the effect is attached to, and adds the effect to the
	 * actor's list of effects
	 *
	 * @param Actor The actor that will be affected by the effect.
	 * @param NameBoneHit The name of the bone that was hit.
	 *
	 * @return A boolean value.
	 */
	virtual bool InitObject(AActor* Actor, FName NameBoneHit);

	/**
	 * This function removes the effect from the actor's effect list, sets the actor to null, and then destroys the effect
	 */
	virtual void DestroyObject();
};

UCLASS()
class RISINGSIGNAL_API URSStateEffectExecuteOnce : public URSStateEffects
{
	GENERATED_BODY()
public:
	/*Value witch will change one of the indicators*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float Power = 20.0f;

	/**
	 * The function calls the parent class's InitObject function, then calls the ExecuteOnce function
	 *
	 * @param Actor The actor that the effect is being applied to.
	 * @param NameBoneHit The name of the bone that was hit.
	 *
	 * @return A boolean value.
	 */
	bool InitObject(AActor* Actor, FName NameBoneHit) override;

	/**
	 * This function is called when the object is destroyed.
	 */
	void DestroyObject() override;

	/**
	 * If the actor has a health component, change its health value by the power of the effect
	 */
	virtual void ExecuteOnce();
};

UCLASS()
class RISINGSIGNAL_API URSStateEffectExecuteTime : public URSStateEffects
{
	GENERATED_BODY()
public:
	/*Value witch will change one of the indicators*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting ExecuteTimer")
	float Power = 20.0f;
	/*Effect cooldown time*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting ExecuteTimer")
	float Timer = 5.0f;
	/*Rate of time effect*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting ExecuteTimer")
	float RateTime = 1.0f;

	/*Timer Handle*/
	FTimerHandle TimerHandle_ExecuteTimer;
	FTimerHandle TimerHandle_EffectTimer;

	/**
	 * It creates a timer that will destroy the object after a certain amount of time, and another timer that will execute the effect every
	 * X seconds
	 *
	 * @param Actor The actor that will be affected by the state.
	 * @param NameBoneHit The name of the bone to attach the particle effect to.
	 *
	 * @return A boolean value.
	 */
	bool InitObject(AActor* Actor, FName NameBoneHit) override;

	/**
	 * If the particle emitter is not null, destroy it
	 */
	void DestroyObject() override;

	/**
	 * If the actor has a health component, change the health value by the power value
	 */
	virtual void Execute();
};
