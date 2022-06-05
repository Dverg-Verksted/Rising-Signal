// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "ActorsComponents/RSHealthComponent.h"
#include "RSCharacterHealthComponent.generated.h"

/**
* Delegate method for broadcast to UMG when Stamina change.
* @param Stamina The value which means a character get tired.
* @note Broadcast when stamina changes.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChange, float, Stamina, float, IncrementValue);

/**
* Actor component for Health and Stamina system.Inherit class. Can be attached to any Actors witch needed stamina.
* @param Stamina The value witch contain time when stamina will recovering.
* @param MaxStaminaValue The value witch contain value incrementing stamina.
* @param StaminaCoolDownRecoverTime The value witch contain time when stamina will recovering.
* @note Parent [RSHealthComponent] -> this [RSCharacterHealthComponent]
*/
UCLASS()
class RISINGSIGNAL_API URSCharacterHealthComponent : public URSHealthComponent
{
	GENERATED_BODY()

protected:
	// The value witch contain default stamina.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Выносливость",
		meta = (DisplayName="Выносливость по умолчанию", AllowPrivateAccess = "true",
			ToolTip = "Параметр времени по истечению которого выносливость начнет восстанавливаться",
			EditConditionHides))
	float Stamina = 100.0f;

	// The value witch contain max stamina.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Здоровье",
	meta = (DisplayName="Максимальное выносливасть", AllowPrivateAccess = "true",
		ToolTip = "Максимально допустимое количество выносливости",
		EditConditionHides))
	float MaxStaminaValue = 100.0f;

public:
	// Delegate
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Выносливасть")
	FOnStaminaChange OnStaminaChange;

	// The pointer timer handle for stamina cool down.
	FTimerHandle TimerHandle_StaminaCoolDown;
	// The pointer timer handle for stamina recover rate time.
	FTimerHandle TimerHandle_StaminaRecoverRateTimer;

	// The value witch contain time when stamina will recovering.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Выносливасть",
		meta = (DisplayName="Время востоновления выносливоси", AllowPrivateAccess = "true",
			ToolTip = "Параметр времени по истечению которого выносливость начнет восстанавливаться",
			EditConditionHides))
	float StaminaCoolDownRecoverTime = 5.0f;

	// The value witch contain value incrementing stamina.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Выносливасть",
		meta = (DisplayName="Значение востовновления", AllowPrivateAccess = "true",
			ToolTip = "Параметр указывает на какое значение будет увеличиваться в промежуток времени выносливость",
			EditConditionHides))
	float StaminaRecoverValue = 1.0f;

	// The value contains the time when stamina will recover by rate.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Выносливасть",
		meta = (DisplayName="Скорость восстановления", AllowPrivateAccess = "true",
			ToolTip = "Параметр указывает на скрость востоновления",
			EditConditionHides))
	float StaminaRecoverRate = 0.1f;

/**
* Getter function for current stamina.
* @return Current stamina.
*/
	float GetCurrentStamina();
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
* Function witch get the stamina value
* @note BlueprintCallable function.
*/
	UFUNCTION(BlueprintCallable)
	float GetStaminaValue();
};
