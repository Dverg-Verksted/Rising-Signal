// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RSEffect.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API URSEffect : public UObject
{
    GENERATED_BODY()

public:

    URSEffect();

    UPROPERTY()
    bool IsDamage = false;
    UPROPERTY()
    float Value = 0.0f;
    UPROPERTY()
    float TimeActive = 0.0f;
    
};
