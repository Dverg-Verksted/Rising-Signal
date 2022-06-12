// It is owned by the company Dverg Verksted.

#pragma once

#include "HealthStruct.h"
#include "CharacterAbilityStruct.generated.h"

/**
 * Structure describing health parameters and ability system for the character
 * @note Inherited RSHealthComponent
 */
USTRUCT(BlueprintType)
struct FCharacterAbilityStruct
{
	GENERATED_BODY()
	// The value witch contain default stamina.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Выносливость",
		meta = (DisplayName = "Выносливость по умолчанию", AllowPrivateAccess = "true",
			ToolTip = "Параметр времени по истечению которого выносливость начнет восстанавливаться", ClampMin = 1.0f, UIMin = 0.0f))
	float Stamina = 100.0f;

	// The value witch contain max stamina.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Здоровье",
		meta = (DisplayName = "Максимальное выносливасть", AllowPrivateAccess = "true",
			ToolTip = "Максимально допустимое количество выносливости", ClampMin = 1.0f, UIMin = 1.0f))
	float MaxStaminaValue = 100.0f;

	// The value witch contain time when stamina will recovering.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Выносливасть",
		meta = (DisplayName = "Время востоновления выносливоси", AllowPrivateAccess = "true",
			ToolTip = "Параметр времени по истечению которого выносливость начнет восстанавливаться", ClampMin = 0.0f, UIMin = 0.0f,
			Units = "times"))
	float StaminaCoolDownRecoverTime = 5.0f;

	// The value witch contain value incrementing stamina.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Выносливасть",
		meta = (DisplayName = "Значение востовновления", AllowPrivateAccess = "true",
			ToolTip = "Параметр указывает на какое значение будет увеличиваться в промежуток времени выносливость", ClampMin = 0.0f,
			UIMin = 0.0f))
	float StaminaRecoverValue = 1.0f;

	// The value contains the time when stamina will recover by rate.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Выносливасть",
		meta = (DisplayName = "Скорость восстановления", AllowPrivateAccess = "true",
			ToolTip = "Параметр указывает на скрость востоновления", ClampMin = 0.0f, UIMin = 0.0f))
	float StaminaRecoverRate = 0.1f;
};
