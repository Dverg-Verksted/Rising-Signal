// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSAbilitySystem.generated.h"

class ARSGamePLayer;

// Enum for ability state types
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

// Struct for creating state in ability system
USTRUCT(BlueprintType)
struct FStateParams
{
    GENERATED_USTRUCT_BODY()

    // Value definition on start and be default
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Значение по умолчанию на старте"))
    float CurrentValue = 0.0f;

    // Max state value
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Максимальное значение параметра"))
    float MaxValue = 100.0f;

    // Min state value
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Минимальное значение параметра"))
    float MinValue = 0.0f;

    // Ability State type 
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Тип параметра"))
    EAbilityStatesType StateType = EAbilityStatesType::Health;

    // Value for Hungry, when system should make damage on health
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Значение, после которого голод будет убавлять здоровье", EditCondition = "StateType == EAbilityStatesType::Hungry", EditConditionHides))
    float AfterIsDebafHungry = 0.0f;

    // Value for Temp, when system should make damage on health
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Значение, после которого замерзание будет убавлять здоровье", EditCondition = "StateType == EAbilityStatesType::Temp", EditConditionHides))
    float AfterIsDebafTemp = 0.0f;
    
    // How much changes state per time
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Значение, на которое будет изменяться значение параметра при восстановлении или убавлении"))
    float ChangedValue = 0.0f;
    
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

    // Set new change value in state in ability system with AbilityStateType
    UFUNCTION()
    void SetChangeValue(EAbilityStatesType AbilityStateType, float ChangedValueModifier);
    
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
    void ChangeCurrentStateValue(EAbilityStatesType StateTy, float AddValue);

    
    /**
     * @brief Need for getting choose state from states in ability system
     * @param AbilityStateType - state`s type which will be returned
     * @return - return State from ability system states
     */
    UFUNCTION(BlueprintCallable)
    FStateParams GetState(EAbilityStatesType AbilityStateType);
    
protected:
    // Called when the game starts
    virtual void BeginPlay() override;

private:
    // control on state changes, it check all state on new change value
    UFUNCTION()
    void CheckStateChanges();

    // need to count how mach Stamina need to change
    UFUNCTION()
    float GetStaminaChangedValue();

    // need to count how mach Health need to change
    UFUNCTION()
    float GetHealthChangedValue();

    // Timer for control how often need to check state changes
    FTimerHandle TStateChange;

    // UPROPERTIES

    // Array of Ability states
    UPROPERTY(EditDefaultsOnly, Category = "Ability states")
    TArray<FStateParams> States;

    UPROPERTY()
    ARSGamePLayer* GamePlayerRef;

    // Value which add plus to stamina state, when it changes
    UPROPERTY(EditDefaultsOnly, Category = "Ability states", meta = (ToolTip = "На сколько изменяется выносливость, если игрок стоит"))
    float ValueStaminaActorStay = 7.0f;
    
    // Value which add plus to stamina state, when it changes
    UPROPERTY(EditDefaultsOnly, Category = "Ability states", meta = (ToolTip = "На сколько изменяется выносливость, если игрок идет"))
    float ValueStaminaActorWalk = 5.0f;

    // Value which add plus to stamina state, when it changes
    UPROPERTY(EditDefaultsOnly, Category = "Ability states", meta = (ToolTip = "На сколько изменяется выносливость, если игрок бежит"))
    float ValueStaminaActorRun = -7.0f;

    // Value for control player dead, хз зачем оно
    UPROPERTY(VisibleDefaultsOnly, Category = "Ability states")
    bool bIsDead = false;

    // Value is how often need update timer
    UPROPERTY(EditDefaultsOnly, Category= "Ability states")
    float TimerChackStateRate = 0.1f;
    
};
