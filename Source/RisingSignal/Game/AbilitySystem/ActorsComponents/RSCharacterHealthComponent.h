#pragma once
#include "CoreMinimal.h"
#include "RisingSignal/Game/AbilitySystem/ActorsComponents/RSHealthComponent.h"
#include "Game/AbilitySystem/ActorsComponents/Structure/CharacterAbilityStruct.h"
#include "RSCharacterHealthComponent.generated.h"

/**
 * Delegate method for broadcast to UMG when Stamina change.
 * @param Stamina The value which means a character get tired.
 * @note Broadcast when stamina changes.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChange, float, Stamina, float, IncrementValue);

/**
 * Delegate method for broadcast to UMG when something from the Ability system changes.
 * @param CharacterAbility The struct witch show current values of the Ability system.
 * @note Broadcast when something changes.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterAbility, FCharacterAbilityStruct, CharacterAbility);

/**
 * Actor component for Health and Stamina system.Inherit class. Can be attached to any Actors witch needed stamina.
 * @param Stamina The value witch contain time when stamina will recovering.
 * @param MaxStaminaValue The value witch contain value incrementing stamina.
 * @param StaminaCoolDownRecoverTime The value witch contain time when stamina will recovering.
 * @note Parent [RSHealthComponent] -> this [RSCharacterHealthComponent]
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent),
	HideCategories = ("Variable", "Tags", "ComponentTick", "ComponentReplication", "Activation", "Cooking", "AssetUserData", "Collision"))
class RISINGSIGNAL_API URSCharacterHealthComponent : public URSHealthComponent
{
	GENERATED_BODY()

protected:
	/*Struct contain all values witch use in the ability system.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Способность",
		meta = (DisplayName = "Настройки выносливости", AllowPrivateAccess = "true",
			ToolTip = "В этой структуре все параметры выносливости"))
	FCharacterAbilityStruct CharacterAbilityStruct;

public:
	// Delegate
	UPROPERTY(BlueprintAssignable)
	FOnStaminaChange OnStaminaChange;

	// Delegate
	UPROPERTY(BlueprintAssignable)
	FOnCharacterAbility OnCharacterAbility;

	// The pointer timer handle for stamina cool down.
	FTimerHandle TimerHandle_StaminaCoolDown;
	// The pointer timer handle for stamina recover rate time.
	FTimerHandle TimerHandle_StaminaRecoverRateTimer;

	/**
	 * Getter function for current stamina.
	 * @return Current stamina.
	 */
	float GetCurrentStamina() { return CharacterAbilityStruct.Stamina; }
	/**
	 * Function broadcast FOnStaminaChange and checking limit values and time recover rate.
	 * @param ChangeValue the new value of increment stamina.
	 */
	void ChangeStaminaValue(float ChangeValue);
	/**
	 * Function cool down the stamina when time ends.
	 */
	void CoolDownStaminaEnd();
	/**
	 * Function recovery the stamina over time.
	 */
	void RecoveryStamina();

	/**
	 * The override function is called when the character's health value changes with new broadcast of dilagate.
	 *
	 * @param ChangeValue The amount of health to change by.
	 */
	virtual void ChangeHealthValue(float ChangeValue) override;

	/**
	 * This is a function that returns the value of the stamina.
	 * @return Stamina value.
	 * @note BlueprintCallable function.
	 */
	UFUNCTION(BlueprintCallable)
	float GetStaminaValue() { return CharacterAbilityStruct.Stamina; }
};
