// It is owned by the company Dverg Verksted.


#include "Game/AI/Wolf/RSAICharacter_Wolf.h"

#include "Game/AI/RSAIController.h"

ARSAICharacter_Wolf::ARSAICharacter_Wolf()
{
}

void ARSAICharacter_Wolf::BeginPlay()
{
    Super::BeginPlay();

    check(AIController);
}

void ARSAICharacter_Wolf::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (AIController)
    {
        if (AIController->GetActorToFocusOn())
        {
            SetNewAIState(EAIState::Threaten);
        }
        else
        {
            SetNewAIState(LastAIState);
        }
    }
}
