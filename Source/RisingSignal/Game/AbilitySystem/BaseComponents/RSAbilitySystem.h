// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "RSEffect.h"
#include "Components/ActorComponent.h"
#include "Trace/Detail/EventNode.h"
#include "RSAbilitySystem.generated.h"

// Delegate for assignment some health changes, return current health
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChanged, float, Health);
// Delegate for call on change health
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeHealth, float, Damage);
//Delegate for getting effects on health value
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAddEffect, bool, IsDamage, float, EffectValue, float, EffectTime);
// Delegate for assignment death event
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSAbilitySystem : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URSAbilitySystem();
    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    UPROPERTY(VisibleAnywhere, BlueprintCallable)
    FAddEffect OnEffectAdd;
    /** Declare delegate @name FHealthChanged
     */
    UPROPERTY(VisibleAnywhere, BlueprintAssignable)
    FHealthChanged HealthChanged;
    /** Declare delegate @name OnChangeHealth
     */
    UPROPERTY(VisibleAnywhere, BlueprintCallable)
    FChangeHealth OnChangeHealthSignature;
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

    // TODO: Maybe erase this func, dont know why it is needed
    // Getter for return current Health value
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetHealth() const {return  Health;}

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
