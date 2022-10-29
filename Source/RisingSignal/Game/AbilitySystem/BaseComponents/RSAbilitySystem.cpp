﻿// It is owned by the company Dverg Verksted.


#include "RSAbilitySystem.h"

#include "TimerManager.h"
#include "../../../../../Plugins/ElectronicNodes/Source/ElectronicNodes/Private/Lib/HotPatch.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/RSGamePLayer.h"


#pragma region Defaults
// Sets default values for this component's properties
URSAbilitySystem::URSAbilitySystem()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void URSAbilitySystem::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(TStateChange, this, &URSAbilitySystem::CheckStateChanges, TimerCheckStateRate, true);
    GamePlayerRef = Cast<ARSGamePLayer>(GetOwner());
    OwnerRef = Cast<ACharacter>(GetOwner());

    if (OwnerRef)
    {
        OwnerRef->OnTakeAnyDamage.AddDynamic(this, &URSAbilitySystem::OnTakeAnyDamageHandle);
    }
    else
    {
        LOG_RS(ELogRSVerb::Error, "No pointer to OwnerRef");
    }
}

#pragma endregion Defaults

#pragma region Functions

void URSAbilitySystem::CheckStateChanges()
{
    for (auto& State : States)
    {
        if (State.StateType == EAbilityStatesType::Health)
        {
            State.ChangedValue = GetHealthChangedValue();
        }
        else if (State.StateType == EAbilityStatesType::Stamina)
        {
            State.ChangedValue = GetStaminaChangedValue();
        }

        State.CurrentValue = FMath::Clamp(State.CurrentValue += State.ChangedValue, State.MinValue, State.MaxValue);

        if (OnStateChangedSignature.IsBound())
        {
            OnStateChangedSignature.Broadcast(State.StateType, State.CurrentValue);
        }
    }
}

float URSAbilitySystem::GetStaminaChangedValue()
{
    if (GamePlayerRef)
    {
        float const CurrentPlayerSpeed = GamePlayerRef->GetVelocity().Size();
        if (CurrentPlayerSpeed <= ValueSpeedWhenPlayerStay)
        {
            return ValueStaminaActorStay * TimerCheckStateRate;
        }
        // if player walk, make decrease stamina
        if (GamePlayerRef->GetDesiredGait() == EAlsGait::Walking)
        {
            return ValueStaminaActorWalk * TimerCheckStateRate;
        }
        // if player run, make more decrease stamina
        if (CurrentPlayerSpeed >= ValueSpeedWhenPlayerRun)
        {
            return ValueStaminaActorRun * TimerCheckStateRate;
        }
    }
    return 0.0f;
}

float URSAbilitySystem::GetHealthChangedValue()
{
    float ValueOnChangeHealth = 0.0f;
    bool bHealthIsCriticalLevel = false;
    for (auto const& State : States)
    {
        if (State.StateType == EAbilityStatesType::Health)
        {
            if (State.CurrentValue <= ValueHealthWhenItCriticalLevel)
            {
                bHealthIsCriticalLevel = true;
            }
        }
    }

    for (auto const& State : States)
    {
        // check relation with hungry state
        if (State.StateType == EAbilityStatesType::Hungry)
        {
            // if player is not hungry, make regeneration hp
            if (State.CurrentValue <= State.AfterIsDebafHungry)
            {
                ValueOnChangeHealth -= State.ChangedValue * TimerCheckStateRate;
            }
            // if player is hungry, make decrease hp
            if (State.CurrentValue <= ValueHungryWhenItNeedToRegeneration && bHealthIsCriticalLevel)
            {
                ValueOnChangeHealth += State.ChangedValue * TimerCheckStateRate;
            }
        }
        // check relation with temperature state
        if (State.StateType == EAbilityStatesType::Temp)
        {
            if (State.CurrentValue >= State.AfterIsDebafTemp)
            {
                ValueOnChangeHealth -= State.ChangedValue * TimerCheckStateRate;
            }
            // if player is hungry, make decrease hp
            if (State.CurrentValue <= ValueTempWhenItNeedToRegeneration && bHealthIsCriticalLevel)
            {
                ValueOnChangeHealth += State.ChangedValue * TimerCheckStateRate;
            }
        }
    }
    return ValueOnChangeHealth;
}

void URSAbilitySystem::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{
    ChangeCurrentStateValue(EAbilityStatesType::Health, -Damage);

    if (GetCurrentStateValue(EAbilityStatesType::Health) <= 0)
    {
        OwnerRef->GetCharacterMovement()->DisableMovement();
        OwnerRef->SetLifeSpan(5);
        OwnerRef->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
        OwnerRef->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        OwnerRef->GetMesh()->SetSimulatePhysics(true);
        OwnerRef->DisableInput(OwnerRef->GetController<APlayerController>());
    }
}

void URSAbilitySystem::SetChangeValue(EAbilityStatesType AbilityStateType, float ChangedValueModifier)
{
    for (auto& State : States)
    {
        if (State.StateType == AbilityStateType)
        {
            State.ChangedValue += ChangedValueModifier;
        }
    }
}

float URSAbilitySystem::GetCurrentStateValue(EAbilityStatesType SearchState) const
{
    for (const auto State : States)
    {
        if (State.StateType == SearchState)
        {
            return State.CurrentValue;
        }
    }
    return -1.0f;
}

void URSAbilitySystem::ChangeCurrentStateValue(EAbilityStatesType StateTy, float AddValue)
{
    for (auto& State : States)
    {
        if (State.StateType == StateTy)
        {
            State.CurrentValue += AddValue;
            State.CurrentValue = FMath::Clamp(State.CurrentValue, State.MinValue, State.MaxValue);
            return;
        }
    }
}

FStateParams URSAbilitySystem::GetState(EAbilityStatesType AbilityStateType)
{
    for (auto& State : States)
    {
        if (State.StateType == AbilityStateType)
        {
            return State;
        }
    }

    return FStateParams();
}

#pragma endregion Functions
