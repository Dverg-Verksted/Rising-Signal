// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Trace/Detail/EventNode.h"
#include "RSAbilitySystem.generated.h"

class ARSGamePLayer;
UENUM(BlueprintType)
enum class EStateType : uint8
{
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
    
    UPROPERTY(EditAnywhere)
    float CurrentValue = 0.0f;

    UPROPERTY(EditAnywhere)
    EStateType StateType = EStateType::Health;

    // How much changes state per second
    UPROPERTY(EditAnywhere)
    float ChangedValue = 0.0f;
    
    UPROPERTY(EditAnywhere)
    float TimeActive = 0.0f;
};

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

#pragma region DeclareDelegate
    /** Declare delegate @name FHealthChanged
     */
    UPROPERTY(BlueprintAssignable)
    FHealthChanged HealthChanged;
    /** Declare delegate @name OnChangeHealthSignature
     */
    UPROPERTY(BlueprintCallable)
    FChangeHealth OnChangeHealthSignature;

    /** Declare delegate @name HungryChanged
     */
    UPROPERTY(BlueprintAssignable)
    FStaminaChanged StaminaChanged;
    /** Declare delegate @name OnChangeHungrySignature 
     */
    UPROPERTY(BlueprintCallable)
    FChangeStamina OnChangeStaminaSignature;

    /** Declare delegate @name StressChanged
     */
    UPROPERTY(BlueprintAssignable)
    FStressChanged StressChanged;
    /** Declare delegate @name OnChangeStressSignature
     */
    UPROPERTY(BlueprintCallable)
    FChangeStress OnChangeStressSignature;
    
    /** Declare delegate @name OnDeath
     */
    UPROPERTY(BlueprintAssignable)
    FOnDeath OnDeath;

#pragma endregion DeclareDelegate 

#pragma region Getters
    // Getter for return current any state in TArray States
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetCurrentStateValue(EStateType SearchState) const;
    
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool GetIsDead() const {return  bIsDead;}
#pragma endregion Getters

    /* Universal func on change any state in TArray States
     * Has a check for the presence of a parameter
     * On input get type of state and change value,
     * if damage (decrease state value) should send parameter with minus
     */
    UFUNCTION(BlueprintCallable)
    void ChangeCurrentStateValue(EStateType StateTy,float ChangesValue);
    
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
    
};
