﻿// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FocusOnPointTask.generated.h"

class ARSAICharacter;

struct FBTRotateToPointTaskMemory
{
    /* AI Character */
    ARSAICharacter* AICharacter;

    /* Yaw Rotation */
    float TargetYawRotation;
};


/**
 * Task to constantly rotate AICharacter to direction to point 
 */
UCLASS()
class RISINGSIGNAL_API UFocusOnPointTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UFocusOnPointTask();

    // Getting data for MemoryNode
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
    FBlackboardKeySelector FocusAimKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim", meta = (ToolTip = "Остаточный угол при котором останавливается вращение"))
    float PrecisionAngle = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim", meta = (ToolTip = "Скорость вращения персонажа"))
    float TurnSpeed = 50;

    virtual uint16 GetInstanceMemorySize() const override;


protected:
    // Rotating character
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
