// It is owned by the company Dverg Verksted.


#include "RSAbilitySystem.h"
#include "TimerManager.h"

// Sets default values for this component's properties
URSAbilitySystem::URSAbilitySystem()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    OnChangeHealth.AddDynamic(this, &URSAbilitySystem::ChangeHealth);
    OnEffectAdd.AddDynamic(this, &URSAbilitySystem::StartEffect);
}

// Called when the game starts
void URSAbilitySystem::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void URSAbilitySystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

// Func for Change Health and call delegate
void URSAbilitySystem::ChangeHealth(float const DamageTaken)
{
    if(!this->bIsDead)
    {
        this->Health -= DamageTaken;
        HealthChanged.Broadcast(this->Health);
        if (this->Health <= 0.0f)
        {
            OnDeath.Broadcast();
            bIsDead = !bIsDead;
        }
    }
}

void URSAbilitySystem::ChangeHealthOnEffects()
{
    this->SumEffectValue = 0;

    //UE_LOG(LogTemp, Warning, TEXT("%f"), Effects[0].Value);
     /*for (URSEffect Effect : this->Effects)
    {
        if (Effect.TimeActive > 0)
        {
            if (Effect.IsDamage)
            {
                SumEffectValue -= Effect.Value;
            }
            else
            {
                SumEffectValue += Effect.Value;
            }    
        }
        Effect.TimeActive--;
    }
    ChangeHealth(SumEffectValue);

    if(SumEffectValue == 0)
    {
        GetOwner()->GetWorldTimerManager().ClearTimer(TEffectChange);
    }
    */
    
}


void URSAbilitySystem::StartEffect(bool IsDamage, float EffectVal, float EffectTime)
{
    if (!NewEffect) return;
    if(!IsDamage)
    {
        EffectVal *= -1;
    }
    
    NewEffect->IsDamage = IsDamage;
    NewEffect->Value = EffectVal;
    NewEffect->TimeActive = EffectTime;
    Effects.Add(NewEffect);
    
    GetOwner()->GetWorldTimerManager().SetTimer(TEffectChange, this, &URSAbilitySystem::ChangeHealthOnEffects, 1.0f, true);
    
    UE_LOG(LogTemp, Warning, TEXT("Is damage %d \n Effect Value %f \n Effect Time %f "),
        IsDamage, EffectVal, EffectTime);
}


