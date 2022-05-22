// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSHealthComponent.generated.h"

/**
  * Delegate method for broadcast to UMG when Health change.
  * @param Health The value which means object health.
  * @param Damage the value witch decrement the Health value.
  * @note Broadcast when health changes.
  */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, Damage);

/**
  * Delegate method for broadcast to UMG when the health value will be less than 0.
  * @note Broadcast when the health value will be less than 0.
  */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

/**
  * The base class for health system for all Actors.Can be attached to any Actors witch health system.
  * @param Health The value witch contain default health.
  * @param MaxHealthValue The value witch contain max health.
  * @param CoefficientDamage The value witch contain coefficient damage.
  */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URSHealthComponent();

	// Delegate
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
	FOnHealthChange OnHealthChange;

	// Delegate
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
	FOnDead OnDead;

	// The value witch contain coefficient damage.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Здоровье",
	meta = (DisplayName="Коэффициент урона", AllowPrivateAccess = "true",
		ToolTip = "Коэффициент получаймого урона",
		EditConditionHides))
	float CoefficientDamage = 1.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// The value witch contain default health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Здоровье",
	meta = (DisplayName="Здоровье по умолчанию", AllowPrivateAccess = "true",
		ToolTip = "Количество здоровья по умолчанию",
		EditConditionHides))
	float Health = 100.0f;

	// The value witch contain max health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Здоровье",
	meta = (DisplayName="Максимальное здоровье", AllowPrivateAccess = "true",
		ToolTip = "Максимально допустимое количество здоровья",
		EditConditionHides))
	float MaxHealthValue = 100.0f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/**
  * Getter function for current health.
  * @return Current health.
  * @note BlueprintCallable function.
  */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth();

/**
  * Setter function for set health value.
  * @param NewHealth the new value of health.
  * @note BlueprintCallable function.
  */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float NewHealth);

/**
  * Function broadcast FOnHealthChange and checking limit values.
  * @param ChangeValue the new value of health.
  * @note BlueprintCallable function.
  */
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void ChangeHealthValue(float ChangeValue);
};
