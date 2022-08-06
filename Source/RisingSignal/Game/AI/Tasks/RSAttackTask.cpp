// It is owned by the company Dverg Verksted.


#include "Game/AI/Tasks/RSAttackTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/AI/RSAICharacter.h"
#include "Kismet/GameplayStatics.h"

URSAttackTask::URSAttackTask()
{
    NodeName = "Attack";
}

EBTNodeResult::Type URSAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    const auto AIController = OwnerComp.GetAIOwner();
    if (!BBComp || !AIController) return EBTNodeResult::Failed;

    ARSAICharacter* Pawn = AIController->GetPawn<ARSAICharacter>();
    if (!Pawn) return EBTNodeResult::Failed;

    AActor* EnemyActor = Cast<AActor>(BBComp->GetValueAsObject(EnemyActorKey.SelectedKeyName));
    if (!EnemyActor) return EBTNodeResult::Failed;

    Pawn->Attack(EnemyActor);

    return EBTNodeResult::Succeeded;
}
