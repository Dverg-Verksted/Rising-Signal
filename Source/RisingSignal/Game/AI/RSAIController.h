// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RSAIController.generated.h"

class URSAIPerceptionComponent;
/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API ARSAIController : public AAIController
{
    GENERATED_BODY()

public:
    ARSAIController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    URSAIPerceptionComponent* RSAIPerceprionComponent;
};
