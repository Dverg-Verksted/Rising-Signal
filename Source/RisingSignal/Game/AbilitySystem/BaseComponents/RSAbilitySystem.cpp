// It is owned by the company Dverg Verksted.


#include "RSAbilitySystem.h"

#include "TimerManager.h"
#include "../../../../../Plugins/ElectronicNodes/Source/ElectronicNodes/Private/Lib/HotPatch.h"
#include "GameInstance/RSGameInstance.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/RSGamePLayer.h"

// Sets default values for this component's properties
URSAbilitySystem::URSAbilitySystem()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    
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
            return 7.0f * TimerChackStateRate;
        }
        // if player walk, make decrease stamina
        if(CurrentPlayerSpeed <= 360)
        {
            return 5.0f * TimerChackStateRate;
        }
        // if player run, make more decrease stamina
        if(CurrentPlayerSpeed >= 410)
        {
            return -7.0f * TimerChackStateRate;
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
                ValueOnChangeHealth -= 1.0f * TimerChackStateRate;
            }
            // if player is hungry, make decrease hp
            if (State.CurrentValue <= 30.0f && bHealthIsCriticalLevel)
            {
                ValueOnChangeHealth += 1.0f * TimerChackStateRate;
            }
        }
        // check relation with temperature state
        if(State.StateType == EAbilityStatesType::Temp)
        {
            if (State.CurrentValue >= State.AfterIsDebafTemp)
            {
                ValueOnChangeHealth -= 5.0f * TimerChackStateRate;
            }
            // if player is hungry, make decrease hp
            if (State.CurrentValue <= 30.0f && bHealthIsCriticalLevel)
            {
                ValueOnChangeHealth += 1.0f * TimerChackStateRate;
            }
        }
    }
    return ValueOnChangeHealth;
}

// Called every frame
void URSAbilitySystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
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


void URSAbilitySystem::ChangeCurrentStateValue(EAbilityStatesType StateTy, float ChangesValue)
{
    for (auto &State : States)
    {
        if (State.StateType == StateTy)
        {
            State.CurrentValue += ChangesValue;
            State.CurrentValue = FMath::Clamp(State.CurrentValue,State.MinValue,State.MaxValue);
            // LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("ChangesValue %f"), ChangesValue));
            return;
        }
    }
}

