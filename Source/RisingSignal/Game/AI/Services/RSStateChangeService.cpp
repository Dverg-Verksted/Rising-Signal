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
                Blackboard->SetValueAsBool(IsIdleKey.SelectedKeyName, true);
                Blackboard->SetValueAsBool(IsPatrolKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsThreatKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsAttackKey.SelectedKeyName, false);

                break;
            }
            case EAIState::Patrol:
            {
                Blackboard->SetValueAsBool(IsIdleKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsPatrolKey.SelectedKeyName, true);
                Blackboard->SetValueAsBool(IsThreatKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsAttackKey.SelectedKeyName, false);
                break;
            }
            case EAIState::Threaten:
            {
                Blackboard->SetValueAsBool(IsIdleKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsPatrolKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsThreatKey.SelectedKeyName, true);
                Blackboard->SetValueAsBool(IsAttackKey.SelectedKeyName, false);
                break;
            }
            case EAIState::Attack:
            {
                Blackboard->SetValueAsBool(IsIdleKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsPatrolKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsThreatKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsAttackKey.SelectedKeyName, true);
                break;
            }
            default:
            {
                Blackboard->SetValueAsBool(IsIdleKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsPatrolKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsThreatKey.SelectedKeyName, false);
                Blackboard->SetValueAsBool(IsAttackKey.SelectedKeyName, false);
            }
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
