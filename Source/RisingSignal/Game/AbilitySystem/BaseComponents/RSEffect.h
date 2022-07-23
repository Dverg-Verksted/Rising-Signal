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

    int GetEffectsNum(){return ArrEffects.Num();}
    
private:

    UPROPERTY()
    FEffect NewEffect;
    
    UPROPERTY()
    TArray<FEffect> ArrEffects;
};
