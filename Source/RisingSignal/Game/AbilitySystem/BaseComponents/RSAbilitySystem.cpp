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
    
    GetWorld()->GetTimerManager().SetTimer(TStateChange, this, &URSAbilitySystem::CheckStateChanges, 1, true);
    GamePlayerRef = Cast<ARSGamePLayer>(GetOwner());
    
}

void URSAbilitySystem::CheckStateChanges()
{
    for (auto &State : States)
    {
        if (State.StateType == EStateType::Health)
        {
            State.ChangedValue = GetHealthChangedValue();
            State.CurrentValue += State.ChangedValue;
            State.CurrentValue = FMath::Clamp(State.CurrentValue,0.0f,100.0f);
            LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Current Health Value %f"), State.CurrentValue));
        }
        if (State.StateType == EStateType::Stamina)
        {
            State.ChangedValue = GetStaminaChangedValue();
            State.CurrentValue += State.ChangedValue;
            State.CurrentValue = FMath::Clamp(State.CurrentValue,0.0f,100.0f);
            LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Current Stamina Value %f"), State.CurrentValue));
        }
        if (State.StateType == EStateType::Hungry)
        {
            State.CurrentValue += State.ChangedValue;
            State.CurrentValue = FMath::Clamp(State.CurrentValue,0.0f,100.0f);
            LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Current Hungry Value %f"), State.CurrentValue));
        }
        
        if (State.StateType == EStateType::Temp)
        {
            State.CurrentValue += State.ChangedValue;
            State.CurrentValue = FMath::Clamp(State.CurrentValue,0.0f,100.0f);
            LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Current Temp Value %f"), State.CurrentValue));
        }
        if (State.StateType == EStateType::Stress)
        {
            State.CurrentValue += State.ChangedValue;
            State.CurrentValue = FMath::Clamp(State.CurrentValue,0.0f,100.0f);
            LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Current Stress Value %f"), State.CurrentValue));
        }
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
            return 3.0f;
        }
        // if player walk, make decrease stamina
        if(CurrentPlayerSpeed <= 350)
        {
            return 5.0f;
        }
        // if player run, make more decrease stamina
        if(CurrentPlayerSpeed >= 450)
        {
            return -20.0f;
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
        // that will work, if hungry and temp comes after
        // may be create one more for before this, and check constraint on
        // on health in it
        if(State.StateType == EStateType::Health)
        {
            if (State.CurrentValue <= 20)
            {
                bHealthIsCriticalLevel = true;
            }
        }
        // check relation with hungry state
        if(State.StateType == EStateType::Hungry)
        {
            // if player is not hungry, make regeneration hp
            if (State.CurrentValue >= 80.0f)
            {
                ValueOnChangeHealth -= 1;
            }
            // if player is hungry, make decrease hp
            if (State.CurrentValue <= 30.0f && bHealthIsCriticalLevel)
            {
                ValueOnChangeHealth += 1;
            }
        }
        // check relation with temperature state
        if(State.StateType == EStateType::Temp)
        {
            if (State.CurrentValue >= 80.0f)
            {
                ValueOnChangeHealth -= 5;
            }
            // if player is hungry, make decrease hp
            if (State.CurrentValue <= 30.0f && bHealthIsCriticalLevel)
            {
                ValueOnChangeHealth += 1;
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

float URSAbilitySystem::GetCurrentStateValue(EStateType SearchState) const
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


void URSAbilitySystem::ChangeCurrentStateValue(EStateType StateTy, float ChangesValue)
{
    for (auto &State : States)
    {
        if (State.StateType == StateTy)
        {
            State.CurrentValue += ChangesValue;
            State.CurrentValue = FMath::Clamp(State.CurrentValue,0.0f,100.0f);
            LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("ChangesValue %f"), ChangesValue));
            return;
        }
    }
}

