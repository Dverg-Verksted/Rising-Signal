// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSAbilitySystem.generated.h"

// Delegate for assignment some health changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChanged, float, Health);
// Delegate for call on change health
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeHealth, float, Damage);
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

    /** Declare delegate @name FHealthChanged
     */
    UPROPERTY(VisibleAnywhere, BlueprintAssignable)
    FHealthChanged HealthChanged;
    /** Declare delegate @name OnChangeHealth
     */
    UPROPERTY(VisibleAnywhere, BlueprintCallable)
    FOnChangeHealth OnChangeHealth;
    /** Declare delegate @name OnDeath
     */
    UPROPERTY(BlueprintAssignable)
    FOnDeath OnDeath;

    /** Func for delegate @name onChangeHealth
     *  Formula: Current health - @param DamageTaken
     *  @param DamageTaken is count of taken damage from anywhere
     */
    UFUNCTION()
    void ChangeHealth(float const DamageTaken);

    // TODO: Maybe erase this func, dont know why it is needed
    // Getter for return current Health value
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetHealth() const {return  Health;}

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

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
