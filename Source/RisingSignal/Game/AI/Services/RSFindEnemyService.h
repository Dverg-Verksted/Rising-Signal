// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RSFindEnemyService.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API URSFindEnemyService : public UBTService
{
    GENERATED_BODY()

public:
    URSFindEnemyService();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
