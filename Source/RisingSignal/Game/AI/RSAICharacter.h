// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RSAICharacter.generated.h"

class ARSAIController;
class URSHealthComponent;
class UBehaviorTree;


UENUM(BlueprintType)
enum EAIState
{
    Idle,
    Patrol,
    Threaten,
    Attack,
    None
};

USTRUCT()
struct FAlertIncreaseData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, meta = (ToolTip = "Отложенный старт повышения уровня угрозы. -1 - не откладывать"))
    float LevelUpDelay = -1;

    UPROPERTY(EditAnywhere, meta = (ToolTip = "Частота повышения уровня угрозы", Units = "s"))
    float LevelUpTimerRate = 0.1;

    UPROPERTY(EditAnywhere, meta = (ToolTip = "На сколько повышается уровень угрозы каждое срабатывание"))
    float LevelUpValue = 1;
};

USTRUCT()
struct FAlertDecreaseData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, meta = (ToolTip = "Отложенный старт понижения уровня угрозы. -1 - не откладывать"))
    float LevelDownDelay = -1;

    UPROPERTY(EditAnywhere, meta = (ToolTip = "Частота повышения уровня угрозы", Units = "s"))
    float LevelDownTimerRate = 0.1;

    UPROPERTY(EditAnywhere, meta = (ToolTip = "На сколько понижается уровень угрозы каждое срабатывание"))
    float LevelDownValue = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIStateChangedSignature, EAIState, NewState, EAIState, OldState);


UCLASS()
class RISINGSIGNAL_API ARSAICharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ARSAICharacter();

    FOnAIStateChangedSignature OnAIStateChanged;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ToolTip = "Дерево поведения персонажа"))
    UBehaviorTree* BehaviorTreeAsset;

    // Set new state for AI Character;
    UFUNCTION(BlueprintCallable, Category = "AI")
    bool SetNewAIState(const EAIState NewState);

    // Get current AI Character state
    UFUNCTION(BlueprintCallable, Category = "AI")
    EAIState GetAIState() const { return CurrentAIState; }

    // Get current Character turn offset
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetTurnOffset() const { return TurnOffset; }

    UFUNCTION()
    virtual void AIStateChanged(EAIState NewState, EAIState PrevState);

    virtual void Attack(AActor* AttackActor);

    UFUNCTION()
    virtual void ProvideDamage(USkeletalMeshComponent* FromMeshComponent);


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    URSHealthComponent* HealthComponent;

    virtual void BeginPlay() override;

    UPROPERTY()
    ARSAIController* AIController;

    EAIState CurrentAIState = EAIState::Patrol;

    EAIState LastAIState = CurrentAIState;

    float LastTurnAngle = 0.0f;

    float TurnOffset = 0.0f;

    void CalculateTurnOffset();

    virtual void EnemyNoticed(bool IsNoticed);

    UPROPERTY(EditAnywhere, Category = "Alert")
    FAlertIncreaseData AlertIncreaseData;

    UPROPERTY(EditAnywhere, Category = "Alert")
    FAlertDecreaseData AlertDecreaseData;

    void IncreaseAlertLevelUpdate();

    void DecreaseAlertLevelUpdate();

    bool TryToIncreaseAlertLevel(float Value);

    bool TryToDecreaseAlertLevel(float Value);

    void SetAlertLevel(float NewAlertLevel);

    UPROPERTY(meta=(ClampMin = 0.0, ClampMax = 100.0))
    float CurrentAlertLevel = 0;

    bool IsAlerted() const;

    FTimerHandle IncreaseAlertLevelTimer;
    FTimerHandle DecreaseAlertLevelTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Base Config")
    float AttackDamage = 50.0;


    // virtual void OnDeath();

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
