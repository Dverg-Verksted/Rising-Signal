// It is owned by the company Dverg Verksted.


#include "Game/AI/Tasks/FocusOnPointTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/AI/RSAICharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Library/RSFunctionLibrary.h"

UFocusOnPointTask::UFocusOnPointTask()
{
    NodeName = "Focus on point";
    bNotifyTick = true;
}

EBTNodeResult::Type UFocusOnPointTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    FBTRotateToPointTaskMemory* TaskMemory = (FBTRotateToPointTaskMemory*)NodeMemory;

    const AAIController* Controller = OwnerComp.GetAIOwner();
    const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    ARSAICharacter* Pawn = Cast<ARSAICharacter>(Controller->GetPawn());
    if (!Pawn) return EBTNodeResult::Failed;

    const FVector PawnLocation = Pawn->GetActorLocation();
    const float PawnRotationYaw = Pawn->GetActorRotation().Yaw;
    const FVector AimLocation = Blackboard->GetValueAsVector(FocusAimKey.SelectedKeyName);

    float YawRotation = (AimLocation - PawnLocation).Rotation().Yaw;

    const float DeltaAngle = FMath::FindDeltaAngleDegrees(PawnRotationYaw, YawRotation);

    if (FMath::Abs(DeltaAngle) <= PrecisionAngle)
        return EBTNodeResult::Succeeded;

    YawRotation = UKismetMathLibrary::NormalizeAxis(PawnRotationYaw + DeltaAngle);

    TaskMemory->StartYawRotation = PawnRotationYaw;
    TaskMemory->EndYawRotation = YawRotation;
    TaskMemory->AICharacter = Pawn;

    return EBTNodeResult::InProgress;
}

uint16 UFocusOnPointTask::GetInstanceMemorySize() const
{
    return sizeof(FBTRotateToPointTaskMemory);
}

void UFocusOnPointTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const FBTRotateToPointTaskMemory* TaskMemory = (FBTRotateToPointTaskMemory*)NodeMemory;

    if (!TaskMemory || !TaskMemory->AICharacter)
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

    const float CurrentRotation = TaskMemory->AICharacter->GetActorRotation().Yaw;
    const float TargetRotation = TaskMemory->EndYawRotation;

    // LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("CurrentYaw = %f, TargetYaw = %f"), CurrentRotation, TargetRotation));

    const float NewYawRotation = FMath::FInterpConstantTo(CurrentRotation, TargetRotation, DeltaSeconds, TurnSpeed);
    const float DeltaYaw = NewYawRotation - TargetRotation;

    TaskMemory->AICharacter->SetActorRotation(FRotator(0, NewYawRotation, 0));

    if (FMath::Abs(DeltaYaw) <= PrecisionAngle)
    {
        // UE_LOG(LogTemp, Warning, TEXT("DeltaYaw = %f, PrecisionAngle = %f"), DeltaYaw, PrecisionAngle);

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
