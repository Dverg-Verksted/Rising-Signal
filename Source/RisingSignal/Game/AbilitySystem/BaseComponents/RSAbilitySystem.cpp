// It is owned by the company Dverg Verksted.


#include "RSAbilitySystem.h"

#include "TimerManager.h"
#include "../../../../../Plugins/ElectronicNodes/Source/ElectronicNodes/Private/Lib/HotPatch.h"
#include "Player/RSGamePLayer.h"

// Sets default values for this component's properties
URSAbilitySystem::URSAbilitySystem()
{
    PrimaryComponentTick.bCanEverTick = false;
    
}

// Called when the game starts
void URSAbilitySystem::BeginPlay()
{
    Super::BeginPlay();
    
    GetWorld()->GetTimerManager().SetTimer(TStateChange, this, &URSAbilitySystem::CheckStateChanges, TimerChackStateRate, true);
    GamePlayerRef = Cast<ARSGamePLayer>(GetOwner());
    
}

void URSAbilitySystem::CheckStateChanges()
{
    for (auto &State : States)
    {
        if (State.StateType == EAbilityStatesType::Health)
        {
            State.ChangedValue = GetHealthChangedValue();
        }
        else if (State.StateType == EAbilityStatesType::Stamina)
        {
            State.ChangedValue = GetStaminaChangedValue();
        }
        
        State.CurrentValue = FMath::Clamp(State.CurrentValue += State.ChangedValue,State.MinValue,State.MaxValue);

        // LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("%f"),State.ChangedValue));
        
        //LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Current %hhd Value %f"), State.StateType ,State.CurrentValue));
        
        if(OnStateChangedSignature.IsBound())
        {
            OnStateChangedSignature.Broadcast(State.StateType, State.CurrentValue);
        }
    }
}

float URSAbilitySystem::GetStaminaChangedValue()
{
    if(GamePlayerRef)
    {
        // add in player pawn enum on movement type - stand, walk, run, jump and etc.
        float const CurrentPlayerSpeed = GamePlayerRef->GetVelocity().Size();
        // this shitcode should rewrite on enum switcher
        // if player stand, make regeneration stamina
        if(CurrentPlayerSpeed <= 0)
        {
            return ValueStaminaActorStay * TimerChackStateRate;
        }
        // if player walk, make decrease stamina
        if(CurrentPlayerSpeed <= 360)
        {
            return ValueStaminaActorWalk * TimerChackStateRate;
        }
        // if player run, make more decrease stamina
        if(CurrentPlayerSpeed >= 410)
        {
            return ValueStaminaActorRun * TimerChackStateRate;
        }
    }
    return 0.0f;
}

float URSAbilitySystem::GetHealthChangedValue()
{
    float ValueOnChangeHealth = 0.0f;
    bool bHealthIsCriticalLevel = false;
    for (auto const &State : States)
    {
        if(State.StateType == EAbilityStatesType::Health)
        {
            if (State.CurrentValue <= 20)
            {
                bHealthIsCriticalLevel = true;
            }
        }
    }
    
    for (auto const &State : States)
    {
        // check relation with hungry state
        if(State.StateType == EAbilityStatesType::Hungry)
        {
            // if player is not hungry, make regeneration hp
            if (State.CurrentValue >= State.AfterIsDebafHungry)
            {
                ValueOnChangeHealth -= State.ChangedValue * TimerChackStateRate;
            }
            // if player is hungry, make decrease hp
            if (State.CurrentValue <= 30.0f && bHealthIsCriticalLevel)
            {
                ValueOnChangeHealth += State.ChangedValue * TimerChackStateRate;
            }
        }
        // check relation with temperature state
        if(State.StateType == EAbilityStatesType::Temp)
        {
            if (State.CurrentValue >= State.AfterIsDebafTemp)
            {
                ValueOnChangeHealth -= State.ChangedValue * TimerChackStateRate;
            }
            // if player is hungry, make decrease hp
            if (State.CurrentValue <= 30.0f && bHealthIsCriticalLevel)
            {
                ValueOnChangeHealth += State.ChangedValue * TimerChackStateRate;
            }
        }
    }
    return ValueOnChangeHealth;
}

void URSAbilitySystem::SetChangeValue(EAbilityStatesType AbilityStateType, float ChangedValueModifier)
{
    for (auto &State : States)
    {
        if(State.StateType == AbilityStateType)
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
    for (auto &State : States)
    {
        if (State.StateType == StateTy)
        {
            State.CurrentValue += AddValue;
            State.CurrentValue = FMath::Clamp(State.CurrentValue,State.MinValue,State.MaxValue);
            // LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("ChangesValue %f"), ChangesValue));
            return;
        }
    }
}

FStateParams URSAbilitySystem::GetState(EAbilityStatesType AbilityStateType)
{
    for (auto &State : States)
    {
        if (State.StateType == AbilityStateType)
        {
            return State;
        }
    }
    FStateParams TempParam;
    return TempParam;
}

