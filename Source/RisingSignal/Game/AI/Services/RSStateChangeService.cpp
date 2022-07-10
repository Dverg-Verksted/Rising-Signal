// It is owned by the company Dverg Verksted.


#include "Game/AI/Services/RSStateChangeService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/AI/RSAICharacter.h"

URSStateChangeService::URSStateChangeService()
{
    NodeName = "On Change State";
}

void URSStateChangeService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Pawn = Cast<ARSAICharacter>(OwnerComp.GetAIOwner()->GetPawn());

        switch (Pawn->GetAIState())
        {
            case EAIState::Idle:
            {
                if (Blackboard->GetValueAsBool(IsPatrollingKey.SelectedKeyName) ||
                    Blackboard->GetValueAsBool(IsAttackingKey.SelectedKeyName))
                {
                    Blackboard->SetValueAsBool(IsPatrollingKey.SelectedKeyName, false);
                    Blackboard->SetValueAsBool(IsAttackingKey.SelectedKeyName, false);
                }
                break;
            }
            case EAIState::Patrol:
            {
                if (!Blackboard->GetValueAsBool(IsPatrollingKey.SelectedKeyName))
                {
                    Blackboard->SetValueAsBool(IsPatrollingKey.SelectedKeyName, true);
                    Blackboard->SetValueAsBool(IsAttackingKey.SelectedKeyName, false);
                }
                break;
            }
            case EAIState::Attack:
            {
                if (!Blackboard->GetValueAsBool(IsAttackingKey.SelectedKeyName))
                {
                    Blackboard->SetValueAsBool(IsPatrollingKey.SelectedKeyName, false);
                    Blackboard->SetValueAsBool(IsAttackingKey.SelectedKeyName, true);
                }
                break;
            }
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
