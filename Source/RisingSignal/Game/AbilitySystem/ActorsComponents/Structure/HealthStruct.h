#pragma once

#include "CoreMinimal.h"
#include "HealthStruct.generated.h"

/**
 * Structure describing health parameters
 */
USTRUCT(BlueprintType)
struct FHealthStruct
{
	GENERATED_BODY()

	// Current health value.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Здоровье",
		meta = (DisplayName = "Здоровье по умолчанию", AllowPrivateAccess = "true", ToolTip = "Количество здоровья по умолчанию",
			ClampMin = 1.0f, UIMin = 1.0f))
	float Health = 100.0f;

	// The value witch contain max health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Здоровье",
		meta = (DisplayName = "Максимальное здоровье", AllowPrivateAccess = "true", ToolTip = "Максимально допустимое количество здоровья",
			ClampMin = 1.0f, UIMin = 1.0f))
	float MaxHealthValue = 100.0f;

	// The value witch contain coefficient damage.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Здоровье",
		meta = (DisplayName = "Коэффициент урона", AllowPrivateAccess = "true", ToolTip = "Коэффициент получаймого урона", ClampMin = 0.0f,
			UIMin = 0.0f))
	float CoefficientDamage = 1.0f;
};
