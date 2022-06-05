#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RisingSignal/Effects/RSStateEffects.h"
#include "RSIGameActor.generated.h"

// This class does not need to be modified.
UINTERFACE()
class URSIGameActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * This is the interface that will be used to implement the functions in the class that inherits from it.
 */
class RISINGSIGNAL_API IRSIGameActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
/**
 * It returns the surface type of the actor.
 * 
 * @return The surface type of the actor.
 */
	virtual EPhysicalSurface GetSurfaceType();

/**
 * It returns an array of all the current effects on the actor.
 * 
 * @return An array of pointers to the class IRSStateEffects.
 */
	virtual TArray<URSStateEffects*> GetAllCurrentEffects();

/**
 * *|CURSOR_MARCADOR|*
 * 
 * @param RemoveEffect The effect to remove.
 */
	virtual void RemoveEffect(URSStateEffects* RemoveEffect);

/**
 * This function adds a new effect to the actor
 * 
 * @param newEffect The effect to add to the actor.
 */
	virtual void AddEffect(URSStateEffects* newEffect);

/**
 * This function returns the location of the effect
 * 
 * @param Effect The effect that is being applied.
 * 
 * @return The location of the effect.
 */
	virtual FVector GetEffectLocation(URSStateEffects* Effect);

/**
 * This function returns the name of the root component of the actor that the effect is attached to
 * 
 * @param Effect The effect that is being applied to the actor.
 * 
 * @return The name of the root component of the actor that the effect is attached to.
 */
	virtual FString GetAttachComponent(URSStateEffects* Effect);
};
