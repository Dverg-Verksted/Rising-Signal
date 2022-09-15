// It is owned by the company Dverg Verksted.


#include "Game/AI/Components/RSAIPerceptionComponent.h"

#include "AIController.h"
//#include "Game/AbilitySystem/ActorsComponents/RSHealthComponent.h"
#include "Library/RSFunctionLibrary.h"
#include "Perception/AISense_Sight.h"
#include "Player/RSGamePLayer.h"

AActor* URSAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    if (PercieveActors.Num() == 0) return nullptr;

    const AAIController* Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;
    for (const auto PercieveActor : PercieveActors)
    {
        const auto AbilityComponent = PercieveActor->FindComponentByClass<URSAbilitySystem>();
        if (Cast<ACharacter>(PercieveActor) && AbilityComponent && !AbilityComponent->GetIsDead())
        // TODO: Check if enemies and change HealthComponent to AbilitySystem
        {
            const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
                // LOG_RS(ELogRSVerb::Display, "New BestPawn = " + BestPawn->GetName());
            }
        }
    }

    if (bShowDebugInfo)
    {
        LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("Distance to closest enemy = %f"), BestDistance));
    }

    // LOG_RS(ELogRSVerb::Display, "BestPawn Set");

    return BestPawn;
}
