// It is owned by the company Dverg Verksted.


#include "Game/AI/Tasks/ChangeStateTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Game/AI/RSAICharacter.h"

UChangeStateTask::UChangeStateTask()
{
    NodeName = "ChangeState";
}

EBTNodeResult::Type UChangeStateTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller) return EBTNodeResult::Failed;

    ARSAICharacter* Pawn = Cast<ARSAICharacter>(OwnerComp.GetOwner());
    if (!Pawn) return EBTNodeResult::Failed;

    Pawn->SetNewAIState(NewAIState);

    return EBTNodeResult::Succeeded;
}
