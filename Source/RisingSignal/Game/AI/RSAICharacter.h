// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RSAICharacter.generated.h"

class ARSBaseWeapon;
class URSAbilitySystem;
class ARSAIController;
class UBehaviorTree;


DECLARE_MULTICAST_DELEGATE_OneParam(FEnemyInSightChangeSignature, bool);


UENUM(BlueprintType)
enum class EAIState : uint8
{
    Idle,
    Patrol,
    Threaten,
    Attack,
    None
};

UENUM()
enum class EAttackType : uint8
{
    None,
    NonWeapon UMETA(DisplayName = "Без оружия"),
    WithWeapon UMETA(DisplayName = "С оружием")
};

USTRUCT()
struct FAttackData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, DisplayName = "Тип атаки")
    EAttackType AttackType = EAttackType::None;

    UPROPERTY(EditAnywhere, DisplayName = "Уникальное название атаки")
    FName AttackName = "";

    UPROPERTY(EditAnywhere, DisplayName = "Класс оружия",
        meta = (EditCondition = "AttackType == EAttackType::WithWeapon", EditConditionHides))
    TSoftClassPtr<ARSBaseWeapon> WeaponClassPtr = nullptr;

    UPROPERTY(EditAnywhere, DisplayName = "Аниммонтаж атаки",
        meta = (EditCondition = "AttackType == EAttackType::NonWeapon", EditConditionHides))
    UAnimMontage* AttackAnimMontage;

    UPROPERTY(EditAnywhere, DisplayName = "Имя сокета",
        meta = (ToolTip = "Имя сокета, относительно которого будет высчитываться дистанция до цели",
            EditCondition = "AttackType == EAttackType::NonWeapon", EditConditionHides))
    FName MeleeAttackSocket = "";

    UPROPERTY(EditAnywhere, DisplayName = "Урон атаки", meta = (EditCondition = "AttackType == EAttackType::NonWeapon", EditConditionHides))
    float AttackDamage = 10.0f;

    UPROPERTY(EditAnywhere, DisplayName = "Дистанция атаки",
        meta = (EditCondition = "AttackType == EAttackType::NonWeapon", EditConditionHides))
    float AttackDistance = 10.0f;
};

USTRUCT()
struct FAlertData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, DisplayName = "Откладывать старт на ",
        meta = (ToolTip = "Отложенный старт изменения уровня угрозы. -1 - не откладывать"))
    float LevelChangeDelay = -1;

    UPROPERTY(EditAnywhere, DisplayName = "Частота изменения уровня",
        meta = (ToolTip = "Частота изменения уровня угрозы", Units = "s"))
    float LevelChangeTimerRate = 0.1;

    UPROPERTY(EditAnywhere, DisplayName = "Величина изменения уровня",
        meta = (ToolTip = "На сколько изменяется уровень угрозы каждое срабатывание"))
    float LevelChangeValue = 1;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIStateChangedSignature, EAIState, NewState, EAIState, OldState);


UCLASS(HideCategories = ("Variable", "Transform", "Sockets", "Shape", "Navigation", "ComponentTick", "Physics", "Tags", "Cooking", "HLOD",
        "Mobile", "Activation", "Component Replication", "Events", "Asset User Data", "Collision"))
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

    UFUNCTION()
    virtual void EnemyInSight(bool IsNoticed);

    virtual void Attack(AActor* ActorToAttack);

    UFUNCTION()
    virtual void ProvideDamage(USkeletalMeshComponent* FromMeshComponent);

    FEnemyInSightChangeSignature OnEnemyInSightChangeSignature;

    UFUNCTION(BlueprintCallable)
    virtual float GetAlertLevel() const { return CurrentAlertLevel; }

    UFUNCTION(BlueprintCallable)
    float GetAlertLevelPercent() const { return CurrentAlertLevel / 100.0f; }


protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    URSAbilitySystem* AbilitySystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RunSpeed = 400;

    virtual void BeginPlay() override;

    UPROPERTY()
    ARSAIController* AIController;

    EAIState CurrentAIState = EAIState::Patrol;

    EAIState LastAIState = CurrentAIState;

    float LastTurnAngle = 0.0f;

    float TurnOffset = 0.0f;

    void CalculateTurnOffset();

    bool IsEnemyInSight = false;

    UPROPERTY(EditAnywhere, DisplayName = "Повышение", Category = "Уровень угрозы")
    FAlertData AlertIncreaseData;

    UPROPERTY(EditAnywhere, DisplayName = "Понижение", Category = "Уровень угрозы")
    FAlertData AlertDecreaseData;

    void IncreaseAlertLevelUpdate();

    void DecreaseAlertLevelUpdate();

    bool TryToIncreaseAlertLevel(float Value);

    bool TryToDecreaseAlertLevel(float Value);

    void SetAlertLevel(float NewAlertLevel);

    float CurrentAlertLevel = 0;

    bool IsAlerted() const;

    UPROPERTY(EditAnywhere, Category = "Alert", meta=(ToolTip = "Через какое время сбросывается тревога"))
    float ClearAlertTime = 5.0f;

    FTimerHandle IncreaseAlertLevelTimer;
    FTimerHandle DecreaseAlertLevelTimer;
    FTimerHandle ClearAlertLevelTimer;

    void ClearAlert();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death | Base Config", meta = (DisplayName = "Аниммонтаж смерти"))
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death | Base Config",
        meta = (DisplayName = "Убивается только огнем?", ToolTip = "Нужно ли сжигать персонажа, чтобы уничтожить окончательно?"))
    bool bDieByFire = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death | Base Config",
        meta=(EditCondition = "bDieByFire", EditConditionHides,
            DisplayName = "Время возрождения", ToolTip = "Через сколько персонаж должен возродиться"))
    float ReviveTime = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death | Base Config",
        meta = (EditCondition = "!bDieByFire", EditConditionHides,
            DisplayName = "Время уничтожения", ToolTip = "Через сколько секунд труп должен исчезнуть"))
    float DestroyTime = 10.0f;

    FTimerHandle ReviveTimerHandle;

    UFUNCTION()
    virtual void OnDeath();

    UFUNCTION()
    virtual void Revive();

    UPROPERTY(EditAnywhere, Category = "Атака")
    TArray<FAttackData> AttackList;

#pragma region DEBUG

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool ShouldNoticePlayer = true;

#pragma endregion DEBUG


public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
