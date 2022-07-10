// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FocusOnPointTask.generated.h"

class ARSAICharacter;

struct FBTRotateToPointTaskMemory
{
    /* AI Character */
    ARSAICharacter* AICharacter;

    /* Yaw Rotation */
    float TargetYawRotation;
};


/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API UFocusOnPointTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UFocusOnPointTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
    FBlackboardKeySelector FocusAimKey;

protected:
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
