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

    GetWorld()->GetTimerManager().SetTimer(TStateChange, this, &URSAbilitySystem::CheckStateChanges, TimerUpdateState, true);

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
    }
    
    for (auto& State : States)
    {
        State.CurrentValue = FMath::Clamp(State.CurrentValue += State.ChangedValue, State.MinValue, State.MaxValue);

        if(State.StateType == EAbilityStatesType::Health && State.CurrentValue == 0) OnDeath.Broadcast();
        
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
        if (CurrentPlayerSpeed <= SpeedStay)
        {
            return StaminaStay * TimerUpdateState;
        }
        // if player walk, make decrease stamina
        if (GamePlayerRef->GetDesiredGait() == EAlsGait::Walking)
        {
            return StaminaWalk * TimerUpdateState;
        }
        // if player run, make more decrease stamina
        if (CurrentPlayerSpeed >= SpeedRun)
        {
            return StaminaRun * TimerUpdateState;
        }
    }
    return 0.0f;
}

float URSAbilitySystem::GetHealthChangedValue()
{
    float ValueOnChangeHealth = 0.0f;
    bool bHealthIsCriticalLevel = false;
    
    if(!GodMode)
    {
        FStateParams TempHungryParam = GetState(EAbilityStatesType::Hungry); 
        FStateParams TempTempParam = GetState(EAbilityStatesType::Temp);
        
        if (GetState(EAbilityStatesType::Health).CurrentValue <= HpCritLvl)
        {
            bHealthIsCriticalLevel = true;
        }
        
        if (TempHungryParam.CurrentValue >= TempHungryParam.AfterIsDebafHungry && !bHealthIsCriticalLevel)
        {
            ValueOnChangeHealth -= 10 * TimerUpdateState;
        }
        
        if (TempTempParam.CurrentValue <= TempTempParam.AfterIsDebafTemp && !bHealthIsCriticalLevel)
        {
            ValueOnChangeHealth -= 10 * TimerUpdateState;
        }
        
        // if (TempHungryParam.CurrentValue < RegenHungry && bHealthIsCriticalLevel)
        // {
        //     ValueOnChangeHealth += 10 * TimerUpdateState;
        // }
        
        // if (TempTempParam.CurrentValue > RegenTemp && bHealthIsCriticalLevel)
        // {
        //     ValueOnChangeHealth += 10 * TimerUpdateState;
        // }
        
    }
    
    return ValueOnChangeHealth;
    
}

void URSAbilitySystem::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{
    
    if(!GodMode)
    {
        ChangeCurrentStateValue(EAbilityStatesType::Health, -Damage);
        if(GetState(EAbilityStatesType::Health).CurrentValue <= HpCritLvl)
        {
            GetWorld()->GetTimerManager().SetTimer(TRegenHealth, this, &URSAbilitySystem::RegenHealth, TimerUpdateState, true);
        }
    }
    
    // Death check
    if (GetCurrentStateValue(EAbilityStatesType::Health) <= 0 && !GodMode)
    {
        GetWorld()->GetTimerManager().ClearTimer(TStateChange);
        OnDeath.Broadcast();
    }
}

void URSAbilitySystem::RegenHealth()
{
    if(GetState(EAbilityStatesType::Health).CurrentValue < HpCritLvl)
    ChangeCurrentStateValue(EAbilityStatesType::Health, 10);
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
