// It is owned by the company Dverg Verksted.


#include "RSAbilitySystem.h"

#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/RSGamePLayer.h"


#pragma region Defaults
// Sets default values for this component's properties
URSAbilitySystem::URSAbilitySystem()
{
    PrimaryComponentTick.bCanEverTick = false;
    SphereCollision = CreateDefaultSubobject<USphereComponent>("StressCollision");
    if(SphereCollision) SphereCollision->InitSphereRadius(100.0f);
}

// Called when the game starts
void URSAbilitySystem::BeginPlay()
{
    Super::BeginPlay();
    
    SphereCollision->SetCollisionProfileName("OverlapAllDynamic");
    SphereCollision->SetGenerateOverlapEvents(true);
    SphereCollision->bHiddenInGame = false;
    SphereCollision->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &URSAbilitySystem::StressOverlapBegin);
    SphereCollision->OnComponentEndOverlap.AddDynamic(this, &URSAbilitySystem::StressOverlapEnd);
    
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
        FStateParams TempStressParam = GetState(EAbilityStatesType::Stress);
        
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

        if (TempStressParam.CurrentValue  >= TempStressParam.AfterIsDebafStress && !bHealthIsCriticalLevel)
        {
            ValueOnChangeHealth -= 10 * TimerUpdateState;
        }
        
        
    }
    
    return ValueOnChangeHealth;
    
}

void URSAbilitySystem::AddEffect(float AddTime, float AddHealth, float AddStamina, float AddStress, float AddHungry, float AddTemp)
{

    if(GetEffect(AddHealth,  AddStamina,  AddStress, AddHungry, AddTemp))
    {
        for (FEffect& Effect : Effects)
        {
            if(Effect.Health == AddHealth && Effect.Stamina == AddStamina &&
                Effect.Stress == AddStress && Effect.Hungry == AddHungry)
            {
                Effect.Time += AddTime;
            }
        }
    }
    else
    {
        if(!GetWorld()->GetTimerManager().IsTimerActive(TEffectCheck))
        {
            GetWorld()->GetTimerManager().SetTimer(TEffectCheck, this,
                &URSAbilitySystem::UpdateEffects, 1.0f, true);
        }
        FEffect Effect = {AddTime, AddHealth,  AddStamina,  AddStress, AddHungry, AddTemp};
        Effects.Add(Effect);
    }
}

bool URSAbilitySystem::GetEffect(float Health, float Stamina, float Stress, float Hungry, float Temp)
{
    FEffect Effect = {Health,  Stamina,  Stress, Hungry, Temp};
    return FindEffect(Effect);
}

void URSAbilitySystem::UpdateEffects()
{
    
    for (FEffect& Effect : Effects)
    {
        ChangeCurrentStateValue(EAbilityStatesType::Health, Effect.Health);
        ChangeCurrentStateValue(EAbilityStatesType::Stamina, Effect.Stamina);
        ChangeCurrentStateValue(EAbilityStatesType::Stress, Effect.Stress);
        ChangeCurrentStateValue(EAbilityStatesType::Hungry, Effect.Hungry);
        ChangeCurrentStateValue(EAbilityStatesType::Temp, Effect.Temp);

        if(Effect.Time != 0.0f)
        {
            Effect.Time--;
        }
        
        if(Effect.Time == 0.0f)
        {
            //Effects.RemoveAt()
        }
    }
}

bool URSAbilitySystem::FindEffect(const FEffect FinEffect)
{
    bool Answer = false;
    for (const FEffect Effect : Effects)
    {
        if(Effect.Health == FinEffect.Health && Effect.Stamina == FinEffect.Stamina &&
                Effect.Stress == FinEffect.Stress && Effect.Hungry == FinEffect.Hungry)
        {
            Answer = true;
        }
    }
    return Answer;
}

void URSAbilitySystem::RemoveEffect(FEffect RemEffect)
{
    if(FindEffect(RemEffect) && Effects.Num() > 0)
    {
        int i = 0;
        for (FEffect Effect : Effects)
        {
            if(Effect.Health == RemEffect.Health && Effect.Stamina == RemEffect.Stamina &&
                Effect.Stress == RemEffect.Stress && Effect.Hungry == RemEffect.Hungry)
            {
                Effects.RemoveAt(i);
                break;
            }
            i++;
        }
    }

    if(Effects.Num() == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(TEffectCheck);
    }
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

void URSAbilitySystem::StressOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UActorComponent* TempAbilitySys = OtherActor->GetComponentByClass(URSAbilitySystem::StaticClass());
    if(OtherActor->GetComponentByClass(URSAbilitySystem::StaticClass()))
    {
        if(!IsMonster)
        {
            SetChangeValue(EAbilityStatesType::Stress,
                Cast<URSAbilitySystem>(TempAbilitySys)->GetState(EAbilityStatesType::Stress).StressDamageOut);
        }
    }
}

void URSAbilitySystem::StressOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
    UActorComponent* TempAbilitySys = OtherActor->GetComponentByClass(URSAbilitySystem::StaticClass());
    if(OtherActor->GetComponentByClass(URSAbilitySystem::StaticClass()))
    {
        if(!IsMonster)
        {
            SetChangeValue(EAbilityStatesType::Stress,
                Cast<URSAbilitySystem>(TempAbilitySys)->
                GetState(EAbilityStatesType::Stress).StressDamageOut * -1.0f);
        }
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
