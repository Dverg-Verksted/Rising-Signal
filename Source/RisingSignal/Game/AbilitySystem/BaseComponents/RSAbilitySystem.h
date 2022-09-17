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
    
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Значение по умолчанию на старте"))
    float CurrentValue = 0.0f;
    
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Максимальное значение параметра"))
    float MaxValue = 100.0f;
    
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Минимальное значение параметра"))
    float MinValue = 0.0f;

    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Тип параметра"))
    EAbilityStatesType StateType = EAbilityStatesType::Health;

    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Значение, после которого голод будет убавлять здоровье", EditCondition = "StateType == EAbilityStatesType::Hungry", EditConditionHides))
    float AfterIsDebafHungry = 0.0f;

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

    //
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
    void ChangeCurrentStateValue(EAbilityStatesType StateTy,float AddValue);

    UFUNCTION(BlueprintCallable)
    FStateParams GetState(EAbilityStatesType AbilityStateType);
    
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

    UPROPERTY(EditDefaultsOnly, Category = "Ability states", meta = (ToolTip = "На сколько изменяется выносливость, если игрок стоит"))
    float ValueStaminaActorStay = 7.0f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Ability states", meta = (ToolTip = "На сколько изменяется выносливость, если игрок идет"))
    float ValueStaminaActorWalk = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Ability states", meta = (ToolTip = "На сколько изменяется выносливость, если игрок бежит"))
    float ValueStaminaActorRun = -7.0f;

    UPROPERTY(VisibleDefaultsOnly, Category = "Ability states")
    bool bIsDead = false;

    UPROPERTY(EditDefaultsOnly, Category= "Ability states")
    float TimerChackStateRate = 0.1f;
    
};
