// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RSAICharacter.generated.h"

class URSHealthComponent;
class UBehaviorTree;


UENUM()
enum EAIState
{
    Idle,
    Patrol,
    Attack
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


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    URSHealthComponent* HealthComponent;

    virtual void BeginPlay() override;

    EAIState CurrentAIState = EAIState::Idle;

    // virtual void OnDeath();


public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
