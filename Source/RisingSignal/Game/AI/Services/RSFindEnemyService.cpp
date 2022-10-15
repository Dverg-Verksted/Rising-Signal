// It is owned by the company Dverg Verksted.


#include "Game/AI/Services/RSFindEnemyService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/AI/Components/RSAIPerceptionComponent.h"
#include "Library/RSFunctionLibrary.h"

URSFindEnemyService::URSFindEnemyService()
{
    NodeName = "Find Enemy";
}

void URSFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = Controller->FindComponentByClass<URSAIPerceptionComponent>();
        if (PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
