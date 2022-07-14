// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "RSAIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API URSAIPerceptionComponent : public UAIPerceptionComponent
{
    GENERATED_BODY()

public:
    AActor* GetClosestEnemy() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (ToolTip = "Отображать дебаг информацию"))
    bool bShowDebugInfo = false;
};
