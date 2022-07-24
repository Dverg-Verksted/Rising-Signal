// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RSEffect.generated.h"

USTRUCT()
struct FEffect
{
    GENERATED_BODY()
    
    UPROPERTY()
    bool IsDamage = false;
    UPROPERTY()
    float Value = 0.0f;
    UPROPERTY()
    float TimeActive = 0.0f;

    // default constructor
    FEffect()
    {
        
    }
};

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API URSEffect : public UObject
{
    GENERATED_BODY()

public:

    URSEffect();

    void AddEffect(bool IsDamage, float EffectVal, float EffectTime);

    float GetEffectSumValue();

    int GetEffectsNum()
    {
        int32 CountActive = 0;
        for (auto &Effect : ArrEffects)
        {
            if (Effect.TimeActive > 0)
            {
                CountActive++;
            }
        }

        return CountActive;
    }
    
private:

    UPROPERTY()
    FEffect NewEffect;
    
    UPROPERTY()
    TArray<FEffect> ArrEffects;
};
