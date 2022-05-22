// It is owned by the company Dverg Verksted.


#include "ActorsComponents/RSCharacterHealthComponent.h"

float URSCharacterHealthComponent::GetCurrentStamina()
{
	return Stamina;
}

void URSCharacterHealthComponent::ChangeStaminaValue(float ChangeValue)
{
	Stamina += ChangeValue;
	OnStaminaChange.Broadcast(Stamina, ChangeValue);

	if (Stamina > MaxStaminaValue)
	{
		Stamina = MaxStaminaValue;
	}
	else
	{
		if(Stamina < 0.0f)
		{
			Stamina = 0.0f;
		}
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_StaminaCoolDown, this,
			&URSCharacterHealthComponent::RecoveryStamina, StaminaCoolDownRecoverTime, false);

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_StaminaRecoverRateTimer);
	}
}

void URSCharacterHealthComponent::CoolDownStaminaEnd()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_StaminaRecoverRateTimer, this,
			&URSCharacterHealthComponent::RecoveryStamina, StaminaRecoverRate, true);
	}
}

void URSCharacterHealthComponent::RecoveryStamina()
{
	float TempleValueStamina = Stamina;
	TempleValueStamina = TempleValueStamina + StaminaRecoverValue;
	if (TempleValueStamina > MaxStaminaValue)
	{
		Stamina = MaxStaminaValue;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_StaminaRecoverRateTimer);
		}
	}
	else
	{
		Stamina = TempleValueStamina;
	}

	OnStaminaChange.Broadcast(Stamina, StaminaRecoverValue);
}

float URSCharacterHealthComponent::GetStaminaValue()
{
	return Stamina;
}
