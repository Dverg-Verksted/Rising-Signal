// It is owned by the company Dverg Verksted.


#include "Game/AI/Tasks/FocusOnPointTask.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
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
    const FRotator PawnRotation = Pawn->GetActorRotation();
    const FVector AimLocation = Blackboard->GetValueAsVector(FocusAimKey.SelectedKeyName);

    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, AimLocation);

    TargetRotation = FRotator(0, TargetRotation.Yaw, 0);

    const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(PawnRotation, TargetRotation);

    const float DeltaAngle = DeltaRotation.Yaw; // FMath::FindDeltaAngleDegrees(PawnRotationYaw, YawRotation);

    if (FMath::Abs(DeltaAngle) <= PrecisionAngle)
        return EBTNodeResult::Succeeded;

    // YawRotation = UKismetMathLibrary::NormalizeAxis(PawnRotationYaw + DeltaAngle);

    DrawDebugLine(GetWorld(), PawnLocation, PawnLocation + PawnRotation.Vector() * 800, FColor::Red, false, 5);

    TaskMemory->StartRotation = PawnRotation;
    TaskMemory->EndRotation = TargetRotation;
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

    const FRotator CurrentRotation = TaskMemory->AICharacter->GetActorRotation();
    const FRotator TargetRotation = TaskMemory->EndRotation;

    // LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("CurrentYaw = %f, TargetYaw = %f"), CurrentRotation, TargetRotation));

    const FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaSeconds, TurnSpeed);
    const float DeltaYaw = NewRotation.Yaw - TargetRotation.Yaw;

    TaskMemory->AICharacter->SetActorRotation(NewRotation);

    if (FMath::Abs(DeltaYaw) <= PrecisionAngle)
    {
        // UE_LOG(LogTemp, Warning, TEXT("DeltaYaw = %f, PrecisionAngle = %f"), DeltaYaw, PrecisionAngle);

        FVector PawnLocation = TaskMemory->AICharacter->GetActorLocation();
        
        DrawDebugLine(GetWorld(), PawnLocation, PawnLocation + CurrentRotation.Vector() * 800, FColor::Red, false, 5);


        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
