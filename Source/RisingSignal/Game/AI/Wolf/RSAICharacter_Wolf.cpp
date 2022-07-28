// It is owned by the company Dverg Verksted.


#include "Game/AI/Wolf/RSAICharacter_Wolf.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/TimelineComponent.h"
#include "Game/AI/RSAIController.h"
#include "Game/AI/Animations/RSBiteAnimNotify.h"
#include "Kismet/GameplayStatics.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/RSGamePLayer.h"

ARSAICharacter_Wolf::ARSAICharacter_Wolf()
{
}

void ARSAICharacter_Wolf::BeginPlay()
{
    Super::BeginPlay();

    check(AIController);
    if (SwitchColorCurve)
    {
        FOnTimelineFloat TimelineProgress;
        TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
        SwitchColorTimeline.AddInterpFloat(SwitchColorCurve, TimelineProgress);
        SwitchColorTimeline.SetLooping(false);
    }

    InitAnimations();
}

void ARSAICharacter_Wolf::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    SwitchColorTimeline.TickTimeline(DeltaSeconds);
}

void ARSAICharacter_Wolf::TimelineProgress(float Value)
{
    GetMesh()->SetScalarParameterValueOnMaterials("SwitchValue", Value);
}


void ARSAICharacter_Wolf::Attack(AActor* AttackActor)
{
    LOG_RS(ELogRSVerb::Warning, "Attack");

    PlayAnimMontage(BiteAnimMontage);
}

void ARSAICharacter_Wolf::ProvideDamage(USkeletalMeshComponent* FromMeshComponent)
{
    if (GetMesh() != FromMeshComponent) return;

    ARSGamePLayer* Player = GetWorld()->GetFirstPlayerController()->GetPawn<ARSGamePLayer>();

    const FVector EnemyLocation = Player->GetActorLocation();

    const FVector JawLocation = FindComponentByClass<USkeletalMeshComponent>()->GetSocketLocation("JawSocket");

    const float DistanceToAttackActor = FVector::Distance(EnemyLocation, JawLocation);

    LOG_RS(ELogRSVerb::Warning, "Distance to enemy = " + FString::SanitizeFloat(DistanceToAttackActor));
    if (DistanceToAttackActor > AttackDistance) return;

    const FRotator BiteDirection = (EnemyLocation - JawLocation).Rotation();

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BloodEffect, EnemyLocation, BiteDirection);

    Player->TakeDamage(AttackDamage, {}, GetController(), this);

    LOG_RS(ELogRSVerb::Warning, "DamageProvided");
}

void ARSAICharacter_Wolf::EnemyNoticed(bool IsNoticed)
{
    Super::EnemyNoticed(IsNoticed);

    if (IsNoticed || CurrentAIState == EAIState::Attack || CurrentAIState == EAIState::Threaten)
    {
        SwitchWolfColorToBlack(true);
    }
    else
    {
        SwitchWolfColorToBlack(false);
    }
}


void ARSAICharacter_Wolf::SwitchWolfColorToBlack(bool bSwitch)
{
    if (bSwitch)
    {
        SwitchColorTimeline.Play();
    }
    else
    {
        SwitchColorTimeline.Reverse();
    }
}

void ARSAICharacter_Wolf::InitAnimations()
{
    if (!BiteAnimMontage) return;

    const auto NotifyEvents = BiteAnimMontage->Notifies;
    for (const auto NotifyEvent : NotifyEvents)
    {
        const auto BiteNotify = Cast<URSBiteAnimNotify>(NotifyEvent.Notify);
        if (BiteNotify)
        {
            BiteNotify->OnNotified.AddUObject(this, &ARSAICharacter_Wolf::ProvideDamage);
            break;
        }
    }
}
