// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RSStateChangeService.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API URSStateChangeService : public UBTService
{
    GENERATED_BODY()

public:
    URSStateChangeService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
    FBlackboardKeySelector IsPatrollingKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
    FBlackboardKeySelector IsAttackingKey;

    

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
