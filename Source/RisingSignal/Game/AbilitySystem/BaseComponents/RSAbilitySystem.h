﻿// It is owned by the company Dverg Verksted.

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
    UPROPERTY(EditDefaultsOnly,
        meta = (ToolTip = "Значение, после которого голод будет убавлять здоровье", EditCondition =
            "StateType == EAbilityStatesType::Hungry", EditConditionHides))
    float AfterIsDebafHungry = 0.0f;

    // Value for Temp, when system should make damage on health
    UPROPERTY(EditDefaultsOnly,
        meta = (ToolTip = "Значение, после которого замерзание будет убавлять здоровье", EditCondition =
            "StateType == EAbilityStatesType::Temp", EditConditionHides))
    float AfterIsDebafTemp = 0.0f;

    // How much changes state per time
    UPROPERTY(EditDefaultsOnly,
        meta = (ToolTip = "Значение, на которое будет изменяться значение параметра при восстановлении или убавлении"))
    float ChangedValue = 0.0f;
    
};

USTRUCT(BlueprintType)
struct FEffect
{
    GENERATED_USTRUCT_BODY()

    float Time;
    float Health;
    float Stamina;
    float Stress;
    float Hungry;
    float Temp;
    
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

#pragma region Defaults

public:
    // Sets default values for this component's properties
    URSAbilitySystem();
    
    UPROPERTY(BlueprintAssignable)
    FOnStateChangedSignature OnStateChangedSignature;

    /** Declare delegate @name OnDeath
     */
    UPROPERTY(BlueprintAssignable)
    FOnDeathSignature OnDeath;

    // Timer for control how often need to check state changes
    FTimerHandle TStateChange;

    // Timer for control Health Regeneration
    FTimerHandle TRegenHealth;

    FTimerHandle TEffectCheck;

    // Just player references for take ability system or some another component/params
    UPROPERTY()
    ARSGamePLayer* GamePlayerRef;

    UPROPERTY()
    ACharacter* OwnerRef;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

#pragma endregion Defaults

#pragma region AbilitySystemParams

private:
    // Array of Ability states
    UPROPERTY(EditDefaultsOnly, Category = "Ability states")
    TArray<FStateParams> States;

    UPROPERTY(EditDefaultsOnly, Category = "Ability states", DisplayName = "Критический уровень здоровья ниже или равен",
        meta = (ToolTip = "Ниже или равно какому значению, у игрока будет критический уровень здоровья"))
    float HpCritLvl = 20.0f;
    

    UPROPERTY(EditDefaultsOnly, Category = "Ability states", DisplayName = "Регенерация здоровья от Сытости",
        meta = (ToolTip = "Ниже или равно какому значению, у игрока будет регенерироваться здоровье от сытости"))
    float RegenHungry = 30.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Ability states", DisplayName = "Регенерация здоровья от Тепла",
        meta = (ToolTip = "Ниже или равно какому значению, у игрока будет регенерироваться здоровье от тепла"))
    float RegenTemp = 30.0f;


    UPROPERTY(EditDefaultsOnly, Category = "Ability states", DisplayName = "Игрок стоит при скорости",
        meta = (ToolTip = "Ниже или равно какой скорости, у игрока будет фиксироваться что он стоит"))
    float SpeedStay = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Ability states", DisplayName = "Игрок идет при скорости",
        meta = (ToolTip = "Ниже или равно какой скорости, у игрока будет фиксироваться ходьба"))
    float SpeedWalk = 360.f;

    UPROPERTY(EditDefaultsOnly, Category = "Ability states", DisplayName = "Игрок бежит при скорости",
        meta = (ToolTip = "Выше или равно какой скорости, у игрока будет фиксироваться бег"))
    float SpeedRun = 410.0f;


    // Value which add plus to stamina state, when it changes
    UPROPERTY(EditDefaultsOnly, Category = "Ability states", DisplayName = "Размер изменения выносливости, если игрок стоит",
        meta = (ToolTip = "На сколько изменяется выносливость, если игрок стоит"))
    float StaminaStay = 7.0f;

    // Value which add plus to stamina state, when it changes
    UPROPERTY(EditDefaultsOnly, Category = "Ability states", DisplayName = "Размер изменения выносливости, если игрок идет",
        meta = (ToolTip = "На сколько изменяется выносливость, если игрок идет"))
    float StaminaWalk = 5.0f;

    // Value which add plus to stamina state, when it changes
    UPROPERTY(EditDefaultsOnly, Category = "Ability states", DisplayName = "Размер изменения выносливости, если игрок бежит",
        meta = (ToolTip = "На сколько изменяется выносливость, если игрок бежит"))
    float StaminaRun = -7.0f;


    // Value for control player dead, хз зачем оно
    UPROPERTY(VisibleDefaultsOnly, Category = "Ability states")
    bool bIsDead = false;

    // Value is how often need update timer
    UPROPERTY(EditDefaultsOnly, Category= "Ability states")
    float TimerUpdateState = 0.1f;

    // if true, player cannot die
    UPROPERTY(EditDefaultsOnly,
            meta = (ToolTip = "Режим бога, ты есть все, ты есть вся"))
    bool GodMode = false;

#pragma endregion AbilitySystemParams

#pragma region Functions

public:
    // Getter for return current any state in TArray States
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetCurrentStateValue(EAbilityStatesType SearchState) const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    FORCEINLINE
    bool GetIsDead() const { return bIsDead; }

    /* Universal func on change any state in TArray States
     * Has a check for the presence of a parameter
     * On input get type of state and change value,
     * if damage (decrease state value) should send parameter with minus
     */
    UFUNCTION(BlueprintCallable)
    void ChangeCurrentStateValue(EAbilityStatesType StateTy, float AddValue);
    
    // Set new change value in state in ability system with AbilityStateType
    UFUNCTION()
    void SetChangeValue(EAbilityStatesType AbilityStateType, float ChangedValueModifier);

    // Return true if player is dead
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool GetIsPlayerDead() const {
        return GetCurrentStateValue(EAbilityStatesType::Health) == 0.0f;
    }

    /**
     * @brief Need for getting choose state from states in ability system
     * @param AbilityStateType - state`s type which will be returned
     * @return - return State from ability system states
     */
    UFUNCTION(BlueprintCallable)
    FStateParams GetState(EAbilityStatesType AbilityStateType);

    UFUNCTION()
    void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
       class AController* InstigatedBy, AActor* DamageCauser);

    void RegenHealth();

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

#pragma endregion Functions

#pragma region Effects

    UPROPERTY(EditDefaultsOnly, Category = "Effect")
    TArray<FEffect> Effects;
     
    /**
     * @brief 
     * @param AddHealth 
     * @param AddStamina 
     * @param AddStress 
     * @param AddHungry 
     * @param AddTemp 
     */
    void AddEffect(float AddTime, float AddHealth, float AddStamina, float AddStress,
        float AddHungry, float AddTemp);

    bool GetEffect(float Health, float Stamina, float Stress, float Hungry, float Temp);

    void UpdateEffects();

    bool FindEffect(FEffect FindEffect);

    void RemoveEffect(FEffect RemEffect);


#pragma endregion Effects
   
};
