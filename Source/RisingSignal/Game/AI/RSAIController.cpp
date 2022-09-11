// It is owned by the company Dverg Verksted.


#include "Game/AI/RSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/RSAIPerceptionComponent.h"
#include "Library/RSFunctionLibrary.h"

ARSAIController::ARSAIController()
{
    RSAIPerceptionComponent = CreateDefaultSubobject<URSAIPerceptionComponent>("RSAIPerceptionComponent");
    SetPerceptionComponent(*RSAIPerceptionComponent);
}

void ARSAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    SetFocus(GetActorToFocusOn());
}

AActor* ARSAIController::GetActorToFocusOn() const
{
    if (!GetBlackboardComponent()) return nullptr;

    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
