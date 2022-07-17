// It is owned by the company Dverg Verksted.


#include "RSAbilitySystem.h"

// Sets default values for this component's properties
URSAbilitySystem::URSAbilitySystem()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    OnChangeHealth.AddDynamic(this, &URSAbilitySystem::ChangeHealth);
}

void URSAbilitySystem::ChangeHealth(float const DamageTaken)
{
    if(!this->bIsDead)
    {
        this->Health -= DamageTaken;
        UE_LOG(LogTemp, Warning, TEXT("Damage Taken %f"),DamageTaken);
        HealthChanged.Broadcast(this->Health);
        if (this->Health <= 0.0f)
        {
            OnDeath.Broadcast();
            bIsDead = !bIsDead;
        }
    }
    
}

// Called when the game starts
void URSAbilitySystem::BeginPlay()
{
    Super::BeginPlay();

    // ...
    
}


// Called every frame
void URSAbilitySystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}



