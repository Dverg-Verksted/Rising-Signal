// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSAbilitySystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSAbilitySystem : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URSAbilitySystem();
    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintCallable)
    FOnChangeHealth OnChangeHealth;
    
    /** func change health value => current health - @param DamageTaken
     *  @param DamageTaken is count of taken damage from anywhere
     */
    UFUNCTION(BlueprintCallable)
    void ChangeHealth(float const DamageTaken);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetHealth() const {return  Health;};

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
    

};
