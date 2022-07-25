// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RSAIController.generated.h"

class URSAIPerceptionComponent;
/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API ARSAIController : public AAIController
{
    GENERATED_BODY()

public:
    ARSAIController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    URSAIPerceptionComponent* RSAIPerceptionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI",
        meta = (ToolTip = "Имя ключа в blackboard для сохранения вражеского актора"))
    FName FocusOnKeyName = "EnemyActor";

    virtual void Tick(float DeltaSeconds) override;

public: // private:

    //Get from blackboard key, actor to focus on
    AActor* GetActorToFocusOn() const;
};
