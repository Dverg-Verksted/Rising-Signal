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
#include "Player/NewTestPlayer/RSBaseCharacter.h"

ARSAICharacter_Wolf::ARSAICharacter_Wolf()
{
}

void ARSAICharacter_Wolf::BeginPlay()
{
    Super::BeginPlay();

    if (!AIController)
    {
        LOG_RS(ELogRSVerb::Error, "No Controller Set for "+GetName());
        return;
    }

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

void ARSAICharacter_Wolf::AIStateChanged(EAIState NewState, EAIState PrevState)
{
    if (NewState == EAIState::Idle || NewState == EAIState::Patrol)
    {
        SwitchWolfColorToBlack(false);
    }
    else
    {
        SwitchWolfColorToBlack(true);
    }

    Super::AIStateChanged(NewState, PrevState);
}

void ARSAICharacter_Wolf::Attack(AActor* ActorToAttack)
{
    LOG_RS(ELogRSVerb::Display, "Attack");

    PlayAnimMontage(BiteAnimMontage);
}

void ARSAICharacter_Wolf::ProvideDamage(USkeletalMeshComponent* FromMeshComponent)
{
    if (GetMesh() != FromMeshComponent) return;

    ARSBaseCharacter* Player = GetWorld()->GetFirstPlayerController()->GetPawn<ARSBaseCharacter>();

    const FVector EnemyLocation = Player->GetActorLocation();

    const FVector JawLocation = FindComponentByClass<USkeletalMeshComponent>()->GetSocketLocation("JawSocket");

    const float DistanceToAttackActor = FVector::Distance(EnemyLocation, JawLocation);

    LOG_RS(ELogRSVerb::Display, "Distance to enemy = " + FString::SanitizeFloat(DistanceToAttackActor));
    if (DistanceToAttackActor > AttackDistance) return;

    const FRotator BiteDirection = (EnemyLocation - JawLocation).Rotation();

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BloodEffect, EnemyLocation, BiteDirection);

    Player->TakeDamage(AttackList[0].AttackDamage, {}, GetController(), this);

    LOG_RS(ELogRSVerb::Display, "DamageProvided");
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
