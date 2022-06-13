#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Game/AbilitySystem/Effects/RSStateEffects.h"
#include "EffectFunctionLibrary.generated.h"

/**
 * This is a blueprint function library. It is a class that contains functions that can be called from blueprints.
 */
UCLASS()
class RISINGSIGNAL_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    /**
     * This function adds an effect to an actor if the effect's possible surface types contains the surface type of the hit location
     *
     * @param TakeEffectActor The actor that will be affected by the effect.
     * @param AddEffectClass The class of the effect you want to add.
     * @param SurfaceType The surface type that the actor hit.
     * @param NameBoneHit The name of the bone that was hit.
     */
    UFUNCTION(BlueprintCallable)
    static void AddEffectBySurfaceType(
        AActor* TakeEffectActor, TSubclassOf<URSStateEffects> AddEffectClass, EPhysicalSurface SurfaceType, FName NameBoneHit);
};
