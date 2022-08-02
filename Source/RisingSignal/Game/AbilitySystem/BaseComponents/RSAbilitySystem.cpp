// It is owned by the company Dverg Verksted.


#include "RSAbilitySystem.h"
#include "TimerManager.h"
#include "Library/RSFunctionLibrary.h"

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

    OnChangeHealthSignature.AddDynamic(this, &URSAbilitySystem::ChangeHealth);

    OnChangeStaminaSignature.AddDynamic(this, &URSAbilitySystem::ChangeStamina);

    OnEffectAddSignature.AddDynamic(this, &URSAbilitySystem::AddEffect);

    OnChangeStressSignature.AddDynamic(this, &URSAbilitySystem::ChangeStress);
    
    EffectSystem = NewObject<URSEffect>(this);
    
}

// Called every frame
void URSAbilitySystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

// Func for Change Health and call delegate
void URSAbilitySystem::ChangeHealth_Implementation(float const DamageTaken)
{
    if(!this->bIsDead)
    {
        this->Health -= DamageTaken;
        if(HealthChanged.IsBound())
        {
            HealthChanged.Broadcast(this->Health);
        }
        if (this->Health <= 0.0f)
        {
            bIsDead = !bIsDead;
            if(OnDeath.IsBound())
            {
                OnDeath.Broadcast();
            }
        }
    }
}

void URSAbilitySystem::ChangeHealthOnEffects()
{
    if(EffectSystem->GetEffectsNum() != 0)
    {
        ChangeHealth(EffectSystem->GetEffectSumValue());
    }
    else
    {
        GetOwner()->GetWorldTimerManager().ClearTimer(TEffectChange);
    }
}

void URSAbilitySystem::ChangeStress_Implementation(float const ChangedValue)
{
    
}

void URSAbilitySystem::ChangeStamina_Implementation(float const ChangedValue)
{
    if (this->Stamina > 0)
    {
        if (this->Stamina - ChangedValue <= 100)
        {
            this->Stamina -= ChangedValue;
        }
        else
        {
            this->Stamina = 100;
        }
        
        if(StaminaChanged.IsBound())
        {
            StaminaChanged.Broadcast(this->Stamina);
        }
    }
    
}

void URSAbilitySystem::AddEffect_Implementation(bool const IsDamage, float const EffectValue, float const EffectTime)
{
    EffectSystem->AddEffect(IsDamage, EffectValue, EffectTime);

    if(!GetOwner()->GetWorldTimerManager().IsTimerActive(TEffectChange))
    {
        GetOwner()->GetWorldTimerManager().SetTimer(TEffectChange, this, &URSAbilitySystem::ChangeHealthOnEffects, 1.0f, true);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Is damage %d \n Effect Value %f \n Effect Time %f "),
        IsDamage, EffectValue, EffectTime);
}


// Add effect to system
// void URSAbilitySystem::AddEffect(bool const IsDamage, float const EffectValue, float const EffectTime)
// {
//     
// }

