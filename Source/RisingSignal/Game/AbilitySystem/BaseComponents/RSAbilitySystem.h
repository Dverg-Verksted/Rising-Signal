// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "RSEffect.h"
#include "Components/ActorComponent.h"
#include "Trace/Detail/EventNode.h"
#include "RSAbilitySystem.generated.h"

#pragma region Delegates
// Delegate for assignment some health changes, return current health value
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChanged, float, Health);
// Delegate for call on change health
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeHealth, float, Damage);

// Delegate for assignment some stamina changes, return current stamina value
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaChanged, float, Stamina);
// Delegate for call on change stamina
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeStamina, float, ChangeValue);

// Delegate for assignment some stress changes, return current stress value
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStressChanged, float, Stress);
// Delegate for call on change stress
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeStress, float, ChangeValue);

//Delegate for getting effects on health value
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAddEffect, bool, IsDamage, float, EffectValue, float, EffectTime);
// Delegate for assignment death event
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
#pragma endregion Delegates

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSAbilitySystem : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URSAbilitySystem();
    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Declare delegate @name OnEffectAdd
     */
    UPROPERTY(VisibleAnywhere, BlueprintCallable)
    FAddEffect OnEffectAddSignature;
    
    /** Declare delegate @name FHealthChanged
     */
    UPROPERTY(VisibleAnywhere, BlueprintAssignable)
    FHealthChanged HealthChanged;
    /** Declare delegate @name OnChangeHealthSignature
     */
    UPROPERTY(VisibleAnywhere, BlueprintCallable)
    FChangeHealth OnChangeHealthSignature;

    /** Declare delegate @name HungryChanged
     */
    UPROPERTY(VisibleAnywhere, BlueprintAssignable)
    FStaminaChanged StaminaChanged;
    /** Declare delegate @name OnChangeHungrySignature 
     */
    UPROPERTY(VisibleAnywhere, BlueprintCallable)
    FChangeStamina OnChangeStaminaSignature;

    /** Declare delegate @name StressChanged
     */
    UPROPERTY(VisibleAnywhere, BlueprintAssignable)
    FStressChanged StressChanged;
    /** Declare delegate @name OnChangeStressSignature
     */
    UPROPERTY(VisibleAnywhere, BlueprintCallable)
    FChangeStress OnChangeStressSignature;
    
    /** Declare delegate @name OnDeath
     */
    UPROPERTY(BlueprintAssignable)
    FOnDeath OnDeath;

    /** Func for delegate @name onChangeHealth
     *  Formula: Current health - @param DamageTaken
     *  @param DamageTaken is count of taken damage from anywhere
     */
    UFUNCTION(BlueprintNativeEvent)
    void ChangeHealth(float const DamageTaken);

    UFUNCTION()
    void ChangeHealthOnEffects();

    UFUNCTION(BlueprintNativeEvent)
    void ChangeStamina(float const ChangedValue);

    UFUNCTION(BlueprintNativeEvent)
    void ChangeStress(float const ChangedValue);

#pragma region Getters
    // Getter for return current Health value
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetHealth() const {return  Health;}
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool GetIsDead() const {return  bIsDead;}
#pragma endregion Getters
    
    // Function for adding effect in effects system
    UFUNCTION(BlueprintNativeEvent)
    void AddEffect(bool const IsDamage, float const EffectValue, float const EffectTime);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

private:

    UPROPERTY()
    URSEffect* EffectSystem;
    
    FTimerHandle TEffectChange;
    
    UPROPERTY()
    float SumEffectValue;
    
    // UPROPERTIES
    UPROPERTY(EditDefaultsOnly, Category = "Ability states")
    bool bNeedHealth;
    UPROPERTY(EditDefaultsOnly, Category = "Ability states", meta = (EditCondition = "bNeedHealth", EditConditionHides))
    float Health;
    
    UPROPERTY(EditDefaultsOnly, Category = "Ability states")
    bool bNeedHungry;
    UPROPERTY(EditDefaultsOnly, Category = "Ability states", meta=(EditCondition = "bNeedHUngry", EditConditionHides))
    float Hungry;
    
    UPROPERTY(EditDefaultsOnly, Category = "Ability states")
    bool bNeedStamina;
    UPROPERTY(EditDefaultsOnly, Category = "Ability states", meta = (EditCondition = "bNeedStamina", EditConditionHides))
    float Stamina;
    
    UPROPERTY(EditDefaultsOnly, Category = "Ability states")
    bool bNeedStress;
    UPROPERTY(EditDefaultsOnly, Category = "Ability states", meta = (EditCondition = "bNeedStress", EditConditionHides))
    float Stress;

    UPROPERTY(VisibleDefaultsOnly, Category = "Ability states")
    bool bIsDead = false;
    
};
