#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/AbilitySystem/ActorsComponents/Structure/HealthStruct.h"
#include "RSHealthComponent.generated.h"

/**
 * Delegate method for ` to UMG when Health change.
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
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),
HideCategories=("Variable", "Tags", "ComponentTick",
"ComponentReplication", "Activation", "Cooking", "AssetUserData", "Collision"))
class RISINGSIGNAL_API URSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URSHealthComponent();

	// Delegate
	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHealthChange;

	// Delegate
	UPROPERTY(BlueprintAssignable)
	FOnDead OnDead;

	/*Struct contain all values witch use in the health.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Здоровье",
	meta = (DisplayName="Настройки здоровья", AllowPrivateAccess = "true",
	ToolTip = "В этой структуре все параметры здоровья"))
	FHealthStruct HealthStruct;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Getter function for current health.
	 * @return Current health.
	 * @note BlueprintCallable function.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth() {return HealthStruct.Health;}

	/**
	 * Setter function for set health value.
	 * @param NewHealth the new value of health.
	 * @note BlueprintCallable function.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float NewHealth) {HealthStruct.Health = NewHealth;}

	/**
	 * Function broadcast FOnHealthChange and checking limit values.
	 * @param ChangeValue the new value of health.
	 * @note BlueprintCallable function.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void ChangeHealthValue(float ChangeValue);
};