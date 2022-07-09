// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Game/AI/RSAICharacter.h"
#include "ChangeStateTask.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API UChangeStateTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UChangeStateTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TEnumAsByte<EAIState> NewAIState;
};
