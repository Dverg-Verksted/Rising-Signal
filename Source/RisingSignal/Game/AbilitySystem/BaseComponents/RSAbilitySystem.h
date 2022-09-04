// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSAbilitySystem.generated.h"

class ARSGamePLayer;

UENUM(BlueprintType)
enum class EAbilityStatesType : uint8
{
    None,
    Health,
    Stamina,
    Stress,
    Hungry,
    Temp
};

USTRUCT(BlueprintType)
struct FStateParams
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY(EditDefaultsOnly)
    float CurrentValue = 0.0f;
    
    UPROPERTY(EditDefaultsOnly)
    float MaxValue = 100.0f;
    
    UPROPERTY(EditDefaultsOnly)
    float MinValue = 0.0f;

    UPROPERTY(EditDefaultsOnly)
    EAbilityStatesType StateType = EAbilityStatesType::Health;

    UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "StateType == EAbilityStatesType::Hungry", EditConditionHides))
    float AfterIsDebafHungry = 0.0f;

    UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "StateType == EAbilityStatesType::Temp", EditConditionHides))
    float AfterIsDebafTemp = 0.0f;
    
    // How much changes state per second
    UPROPERTY(EditDefaultsOnly)
    float ChangedValue = 0.0f;
    
    UPROPERTY(EditDefaultsOnly)
    float TimeActive = 0.0f;
};

#pragma region Delegates
// Universal delegate for all changed Params
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateChangedSignature, EAbilityStatesType, StateType, float, NewValue);

// Delegate for assignment death event
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
#pragma endregion Delegates

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSAbilitySystem : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URSAbilitySystem();
    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region DeclareDelegate

    UPROPERTY(BlueprintAssignable)
    FOnStateChangedSignature OnStateChangedSignature;
    
    /** Declare delegate @name OnDeath
     */
    UPROPERTY(BlueprintAssignable)
    FOnDeathSignature OnDeath;

#pragma endregion DeclareDelegate 

#pragma region Getters
    // Getter for return current any state in TArray States
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetCurrentStateValue(EAbilityStatesType SearchState) const;
    
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool GetIsDead() const {return  bIsDead;}
#pragma endregion Getters

    /* Universal func on change any state in TArray States
     * Has a check for the presence of a parameter
     * On input get type of state and change value,
     * if damage (decrease state value) should send parameter with minus
     */
    UFUNCTION(BlueprintCallable)
    void ChangeCurrentStateValue(EAbilityStatesType StateTy,float ChangesValue);
    
protected:
    // Called when the game starts
    virtual void BeginPlay() override;

private:
    // control on state changes, it check all state on new change value
    UFUNCTION()
    void CheckStateChanges();

    //
    UFUNCTION()
    float GetStaminaChangedValue();

    //
    UFUNCTION()
    float GetHealthChangedValue();
    
    FTimerHandle TStateChange;

    // UPROPERTIES
    
    UPROPERTY(EditDefaultsOnly, Category = "Ability states")
    TArray<FStateParams> States;

    UPROPERTY()
    ARSGamePLayer* GamePlayerRef;
    
    UPROPERTY(VisibleDefaultsOnly, Category = "Ability states")
    bool bIsDead = false;

    UPROPERTY(EditDefaultsOnly, Category= "Ability states")
    float TimerChackStateRate = 0.1f;
    
};
