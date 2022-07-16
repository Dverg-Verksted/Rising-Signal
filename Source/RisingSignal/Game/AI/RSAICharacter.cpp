// It is owned by the company Dverg Verksted.


#include "Game/AI/RSAICharacter.h"

#include "RSAIController.h"
#include "Game/AbilitySystem/ActorsComponents/RSHealthComponent.h"
#include "Library/RSFunctionLibrary.h"

// Sets default values
ARSAICharacter::ARSAICharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ARSAIController::StaticClass();

    HealthComponent = CreateDefaultSubobject<URSHealthComponent>(TEXT("HealthComponent"));
}

bool ARSAICharacter::SetNewAIState(const EAIState NewState)
{
    if (NewState == CurrentAIState)
        return false;

    LastAIState = CurrentAIState;

    CurrentAIState = NewState;
    OnAIStateChanged.Broadcast(NewState, LastAIState);
    return true;
}


void ARSAICharacter::BeginPlay()
{
    Super::BeginPlay();

    AIController = Cast<ARSAIController>(GetController());
    if (AIController)
    {
        AIController->RunBehaviorTree(BehaviorTreeAsset);
    }
    else
    {
        LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("Wrong controller assigned to %s"), *GetName()));
    }
}

void ARSAICharacter::CalculateTurnOffset()
{
    const float NewTurnAngle = GetActorRotation().Yaw;

    const float NewTurnOffset = LastTurnAngle - NewTurnAngle;

    LastTurnAngle = GetActorRotation().Yaw;

    TurnOffset = NewTurnOffset;
    // UE_LOG(LogTemp, Warning, TEXT("%f"), TurnOffset);
}

// Called every frame
void ARSAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CalculateTurnOffset();
}
