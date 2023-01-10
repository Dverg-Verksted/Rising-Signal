// It is owned by the company Dverg Verksted.


#include "RSAbilitySystem.h"

#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/RSGamePLayer.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"


#pragma region Defaults
// Sets default values for this component's properties
URSAbilitySystem::URSAbilitySystem()
{
    PrimaryComponentTick.bCanEverTick = false;
    SphereCollision = CreateDefaultSubobject<USphereComponent>("StressCollision");
    if (SphereCollision) SphereCollision->InitSphereRadius(100.0f);
}

// Called when the game starts
void URSAbilitySystem::BeginPlay()
{
    Super::BeginPlay();

    SphereCollision->SetCollisionProfileName("OverlapAllDynamic");
    SphereCollision->SetGenerateOverlapEvents(true);
    SphereCollision->bHiddenInGame = true;
    SphereCollision->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &URSAbilitySystem::StressOverlapBegin);
    SphereCollision->OnComponentEndOverlap.AddDynamic(this, &URSAbilitySystem::StressOverlapEnd);

    GetWorld()->GetTimerManager().SetTimer(TStateChange, this, &URSAbilitySystem::CheckStateChanges, TimerUpdateState, true);

    GetWorld()->GetTimerManager().SetTimer(TEffectCheck, this, &URSAbilitySystem::UpdateEffects, 1.0f, true);

    GamePlayerRef = Cast<ARSBaseCharacter>(GetOwner());
    OwnerRef = Cast<ACharacter>(GetOwner());

    if (OwnerRef)
    {
        OwnerRef->OnTakeAnyDamage.AddDynamic(this, &URSAbilitySystem::OnTakeAnyDamageHandle);
    }
    else
    {
        LOG_RS(ELogRSVerb::Error, "No pointer to OwnerRef");
    }

    Effects.Add(FEffect{0, EAbilityStatesType::Health, 0});
    Effects.Add(FEffect{0, EAbilityStatesType::Hungry, 0});
    Effects.Add(FEffect{0, EAbilityStatesType::Stamina, 0});
    Effects.Add(FEffect{0, EAbilityStatesType::Stress, 0});
    Effects.Add(FEffect{0, EAbilityStatesType::Temp, 0});

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

        if (!bIsDead && State.StateType == EAbilityStatesType::Health && State.CurrentValue <= 0)
        {
            bIsDead = true;
            OnDeath.Broadcast();
        }

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
        if (CurrentPlayerSpeed > SpeedStay)
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

    if (!GodMode)
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
            ValueOnChangeHealth -= TempHungryParam.HungryDamage * TimerUpdateState;
        }

        if (TempTempParam.CurrentValue <= TempTempParam.AfterIsDebafTemp && !bHealthIsCriticalLevel)
        {
            ValueOnChangeHealth -= TempTempParam.TempDamage * TimerUpdateState;
        }

        if (TempStressParam.CurrentValue >= TempStressParam.AfterIsDebafStress && !bHealthIsCriticalLevel)
        {
            ValueOnChangeHealth -= TempStressParam.StressDamageSelf * TimerUpdateState;
        }

    }

    return ValueOnChangeHealth;

}

void URSAbilitySystem::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{

    if (GodMode || bIsDead) return;

    ChangeCurrentStateValue(EAbilityStatesType::Health, -Damage);
    if (GetState(EAbilityStatesType::Health).CurrentValue <= HpCritLvl)
    {
        GetWorld()->GetTimerManager().SetTimer(TRegenHealth, this, &URSAbilitySystem::RegenHealth, TimerUpdateState, true);
    }

    // Death check
    if (GetCurrentStateValue(EAbilityStatesType::Health) <= 0)
    {
        bIsDead = true;

        GetWorld()->GetTimerManager().ClearTimer(TStateChange);
        OnDeath.Broadcast();
    }
}

void URSAbilitySystem::RegenHealth()
{
    if (GetState(EAbilityStatesType::Health).CurrentValue < HpCritLvl)
        ChangeCurrentStateValue(EAbilityStatesType::Health, 10);
}

void URSAbilitySystem::StressOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UActorComponent* TempAbilitySys = OtherActor->GetComponentByClass(URSAbilitySystem::StaticClass());
    if (OtherActor->GetComponentByClass(URSAbilitySystem::StaticClass()))
    {
        if (!IsMonster)
        {
            SetChangeValue(EAbilityStatesType::Stress,
                Cast<URSAbilitySystem>(TempAbilitySys)->GetState(EAbilityStatesType::Stress).StressDamageOut);
        }
    }
}

void URSAbilitySystem::StressOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UActorComponent* TempAbilitySys = OtherActor->GetComponentByClass(URSAbilitySystem::StaticClass());
    if (OtherActor->GetComponentByClass(URSAbilitySystem::StaticClass()))
    {
        if (!IsMonster)
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

void URSAbilitySystem::SetState(FStateParams StateParams)
{
    for (auto& State : States)
    {
        if (State.StateType == StateParams.StateType)
        {
            State.CurrentValue = StateParams.CurrentValue;
            State.ChangedValue = StateParams.ChangedValue;
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

#pragma region Effects

void URSAbilitySystem::AddEffect(int AddTime, EAbilityStatesType AddEffectType, float AddValue)
{

    for (FEffect& Effect : Effects)
    {
        if (Effect.EffectType == AddEffectType)
        {
            Effect.EffectValue += AddValue;
            Effect.Time += AddTime;
        }
    }
}

void URSAbilitySystem::UpdateEffects()
{
    if (bIsDead) return;

    for (FEffect& Effect : Effects)
    {
        if (Effect.Time != 0.0f)
        {
            ChangeCurrentStateValue(Effect.EffectType, Effect.EffectValue);

            LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("%f"), Effect.EffectValue));

            Effect.Time--;
        }

        if (Effect.Time == 0.0f)
        {
            Effect.Time = 0.0f;
            Effect.EffectValue = 0.0f;
        }
    }
}

#pragma endregion Effects
