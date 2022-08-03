// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RSAttackTask.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API URSAttackTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    URSAttackTask();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    FBlackboardKeySelector EnemyActorKey;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
