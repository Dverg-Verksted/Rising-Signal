#include "RisingSignal/Game/AbilitySystem/ActorsComponents/RSCharacterHealthComponent.h"

void URSCharacterHealthComponent::ChangeStaminaValue(float ChangeValue)
{
    CharacterAbilityStruct.Stamina += ChangeValue;
    OnStaminaChange.Broadcast(CharacterAbilityStruct.Stamina, ChangeValue);
    OnCharacterAbility.Broadcast(CharacterAbilityStruct);

    if (CharacterAbilityStruct.Stamina > CharacterAbilityStruct.MaxStaminaValue)
    {
        CharacterAbilityStruct.Stamina = CharacterAbilityStruct.MaxStaminaValue;
    }
    else
    {
        if (CharacterAbilityStruct.Stamina < 0.0f)
        {
            CharacterAbilityStruct.Stamina = 0.0f;
        }
    }

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_StaminaCoolDown, this, &URSCharacterHealthComponent::RecoveryStamina,
            CharacterAbilityStruct.StaminaCoolDownRecoverTime, false);

        GetWorld()->GetTimerManager().ClearTimer(TimerHandle_StaminaRecoverRateTimer);
    }
}

void URSCharacterHealthComponent::CoolDownStaminaEnd()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_StaminaRecoverRateTimer, this, &URSCharacterHealthComponent::RecoveryStamina,
            CharacterAbilityStruct.StaminaRecoverRate, true);
    }
}

void URSCharacterHealthComponent::RecoveryStamina()
{
    float TempleValueStamina = CharacterAbilityStruct.Stamina;
    TempleValueStamina = TempleValueStamina + CharacterAbilityStruct.StaminaRecoverValue;
    if (TempleValueStamina > CharacterAbilityStruct.MaxStaminaValue)
    {
        CharacterAbilityStruct.Stamina = CharacterAbilityStruct.MaxStaminaValue;
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(TimerHandle_StaminaRecoverRateTimer);
        }
    }
    else
    {
        CharacterAbilityStruct.Stamina = TempleValueStamina;
    }

    OnStaminaChange.Broadcast(CharacterAbilityStruct.Stamina, CharacterAbilityStruct.StaminaRecoverValue);
    OnCharacterAbility.Broadcast(CharacterAbilityStruct);
}

void URSCharacterHealthComponent::ChangeHealthValue(float ChangeValue)
{
    Super::ChangeHealthValue(ChangeValue);
    OnCharacterAbility.Broadcast(CharacterAbilityStruct);
}
