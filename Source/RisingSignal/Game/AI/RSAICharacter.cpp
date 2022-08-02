// It is owned by the company Dverg Verksted.


#include "Game/AI/RSAICharacter.h"

#include "RSAIController.h"
#include "Game/AbilitySystem/ActorsComponents/RSHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Library/RSFunctionLibrary.h"

// Sets default values
ARSAICharacter::ARSAICharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ARSAIController::StaticClass();

    HealthComponent = CreateDefaultSubobject<URSHealthComponent>(TEXT("HealthComponent"));

    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

bool ARSAICharacter::SetNewAIState(const EAIState NewState)
{
    if (NewState == CurrentAIState)
        return false;

    if (CurrentAIState != EAIState::Threaten || CurrentAIState != EAIState::Attack)
        LastAIState = CurrentAIState;

    CurrentAIState = NewState;
    OnAIStateChanged.Broadcast(NewState, LastAIState);
    return true;
}

void ARSAICharacter::AIStateChanged(EAIState NewState, EAIState PrevState)
{
}

void ARSAICharacter::Attack(AActor* AttackActor)
{
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

    OnAIStateChanged.AddDynamic(this, &ARSAICharacter::AIStateChanged);

    OnEnemyInSightChangeSignature.AddUObject(this, &ARSAICharacter::EnemyInSight);
}

void ARSAICharacter::CalculateTurnOffset()
{
    const float NewTurnAngle = GetActorRotation().Yaw;

    const float NewTurnOffset = LastTurnAngle - NewTurnAngle;

    LastTurnAngle = GetActorRotation().Yaw;

    TurnOffset = NewTurnOffset;
}

void ARSAICharacter::EnemyInSight(bool IsNoticed)
{
    if (IsNoticed)
    {
        if (!IsAlerted())
        {
            LOG_RS(ELogRSVerb::Warning, "Enemy Found!");

            SetNewAIState(EAIState::Threaten);

            GetWorldTimerManager().ClearTimer(DecreaseAlertLevelTimer);

            GetWorldTimerManager().SetTimer(
                IncreaseAlertLevelTimer,
                this,
                &ARSAICharacter::IncreaseAlertLevelUpdate,
                AlertIncreaseData.LevelUpTimerRate,
                true,
                AlertIncreaseData.LevelUpDelay
                );
        }
        else
        {
            GetWorldTimerManager().ClearTimer(ClearAlertLevelTimer);
        }
    }
    else
    {
        LOG_RS(ELogRSVerb::Warning, "No Enemy!");
        
        if (IsAlerted())
        {
            GetWorldTimerManager().SetTimer(ClearAlertLevelTimer, this, &ARSAICharacter::ClearAlert, ClearAlertTime);
        }
        else
        {
            GetWorldTimerManager().ClearTimer(IncreaseAlertLevelTimer);

            GetWorldTimerManager().SetTimer(
                DecreaseAlertLevelTimer,
                this,
                &ARSAICharacter::DecreaseAlertLevelUpdate,
                AlertDecreaseData.LevelDownTimerRate,
                true,
                AlertDecreaseData.LevelDownDelay
                );
        }
    }
}

void ARSAICharacter::IncreaseAlertLevelUpdate()
{
    if (IsAlerted())
    {
        GetWorldTimerManager().ClearTimer(IncreaseAlertLevelTimer);
        SetNewAIState(EAIState::Attack);
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
        GetCharacterMovement()->RotationRate = FRotator(250);
        return;
    }

    TryToIncreaseAlertLevel(AlertIncreaseData.LevelUpValue);
}

void ARSAICharacter::DecreaseAlertLevelUpdate()
{
    if (FMath::IsNearlyZero(CurrentAlertLevel))
    {
        GetWorldTimerManager().ClearTimer(DecreaseAlertLevelTimer);
        SetNewAIState(Patrol);
        return;
    }

    TryToDecreaseAlertLevel(AlertDecreaseData.LevelDownValue);
}

bool ARSAICharacter::TryToIncreaseAlertLevel(float Value)
{
    if (IsAlerted()) return false;

    SetAlertLevel(CurrentAlertLevel + Value);
    return true;
}

bool ARSAICharacter::TryToDecreaseAlertLevel(float Value)
{
    if (IsAlerted()) return false;

    SetAlertLevel(CurrentAlertLevel - Value);
    return true;
}

void ARSAICharacter::SetAlertLevel(float NewAlertLevel)
{
    CurrentAlertLevel = FMath::Clamp(NewAlertLevel, 0.0f, 100.0f);

    //LOG_RS(ELogRSVerb::Warning, "Alert Level = " + FString::SanitizeFloat(CurrentAlertLevel));
}

bool ARSAICharacter::IsAlerted() const
{
    return FMath::IsNearlyEqual(CurrentAlertLevel, 100.0f);
}

void ARSAICharacter::ProvideDamage(USkeletalMeshComponent* FromMeshComponent)
{
}

void ARSAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CalculateTurnOffset();

    if (AIController)
    {
        if (AIController->GetActorToFocusOn() && !IsEnemyInSight)
        {
            IsEnemyInSight = true;
            OnEnemyInSightChangeSignature.Broadcast(IsEnemyInSight);
        }
        else if (!AIController->GetActorToFocusOn() && IsEnemyInSight)
        {
            IsEnemyInSight = false;
            OnEnemyInSightChangeSignature.Broadcast(IsEnemyInSight);
        }
    }
}

void ARSAICharacter::ClearAlert()
{
    SetAlertLevel(0.0f);
    SetNewAIState(EAIState::Patrol);
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->RotationRate = FRotator(50);
    LOG_RS(ELogRSVerb::Warning, "Alert cleared");
}
