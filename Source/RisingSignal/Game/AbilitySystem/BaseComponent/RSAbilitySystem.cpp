// It is owned by the company Dverg Verksted.


#include "RSAbilitySystem.h"


// Sets default values for this component's properties
URSAbilitySystem::URSAbilitySystem()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
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

void URSAbilitySystem::ChangeHealth(float const DamageTaken)
{
    this->Health -= DamageTaken;
    UE_LOG(LogTemp, Display, TEXT("%f"), DamageTaken);
}


