// It is owned by the company Dverg Verksted.


#include "RSHealthComponent.h"


// Sets default values for this component's properties
URSHealthComponent::URSHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URSHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float URSHealthComponent::GetCurrentHealth()
{
	return Health;
}

void URSHealthComponent::SetCurrentHealth(float NewHealth)
{
	Health = NewHealth;
}

void URSHealthComponent::ChangeHealthValue(float ChangeValue)
{
	ChangeValue = ChangeValue * CoefficientDamage;
	Health += ChangeValue;
	OnHealthChange.Broadcast(Health, ChangeValue);

	if (Health > MaxHealthValue)
	{
		Health = MaxHealthValue;
	}
	else
	{
		if (Health < 0.0f)
		{
			OnDead.Broadcast();
		}
	}
}

