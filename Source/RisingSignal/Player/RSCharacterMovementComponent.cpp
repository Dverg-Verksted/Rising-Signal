// It is owned by the company Dverg Verksted.


#include "Player/RSCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "NewTestPlayer/RSBaseCharacter.h"
#include "Game/InteractSystem/Environment/Rope/Rope.h"
#include "Kismet/KismetMathLibrary.h"

void URSCharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay();
    BaseCharacterOwner = StaticCast<ARSBaseCharacter*>(GetOwner());
}

float URSCharacterMovementComponent::GetMaxSpeed() const
{
    float Result = Super::GetMaxSpeed();
    if(BaseCharacterOwner->GetIsSprinting())
    {
        Result = SprintSpeed;
    }
    if(IsCrouching())
    {
        Result = CrouchSpeed;
    }
    if(IsOnLadder())
    {
        Result = ClimbingOnLadderSpeed;
    }
    if(IsOnWall())
    {
        Result = MaxSpeedOnWall;
    }

    return Result;
}

void URSCharacterMovementComponent::StartMantle(const FMantlingMovementParameters& MantlingMovementParameters)
{
    CurrentMantlingParameters = MantlingMovementParameters;
    SetMovementMode(MOVE_Custom, StaticCast<uint8>(ECustomMovementMode::CMOVE_Mantling));
}

void URSCharacterMovementComponent::EndMantle()
{
    SetMovementMode(MOVE_Walking);
}

void URSCharacterMovementComponent::StartRoll()
{
    const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();
    const float OldUnscaledRadius = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
    const float OldUnscaledHalfHeight = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
    const float ClampedRollHalfHeight = FMath::Max3(0.f, OldUnscaledRadius, RollCapsuleHalfHeight);
    CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(OldUnscaledRadius, ClampedRollHalfHeight);
    float HalfHeightAdjust = OldUnscaledHalfHeight - RollCapsuleHalfHeight;
    const float ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;
    UpdatedComponent->MoveComponent(FVector(0.f, 0.f, -ScaledHalfHeightAdjust), UpdatedComponent->GetComponentQuat(), true, nullptr,
        MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);

    const ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
    HalfHeightAdjust = (DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - RollCapsuleHalfHeight - ClampedRollHalfHeight);
    BaseCharacterOwner->SetIsRolling(true);
    float MinRange;
    float MaxRange;
    
    CurrentRollingParameters.RollingCurve->GetTimeRange(MinRange, MaxRange);
    CurrentRollingParameters.Duration = MaxRange - MinRange;

    BaseCharacterOwner->OnStartRoll(HalfHeightAdjust);
    SetMovementMode(MOVE_Custom, StaticCast<uint8>(ECustomMovementMode::CMOVE_Rolling));
}

void URSCharacterMovementComponent::StopRoll()
{
    const ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
    const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();
    const float DefaultHalfHeight = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
    const float DefaultRadius = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
    float HalfHeightAdjust = DefaultHalfHeight - RollCapsuleHalfHeight;
    const float ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;
    
    CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultRadius, DefaultHalfHeight);
    BaseCharacterOwner->SetIsRolling(false);
    if (IsEnoughSpaceToStandUp())
    {
        HalfHeightAdjust = DefaultHalfHeight - CrouchedHalfHeight;
        Crouch();
        BaseCharacterOwner->OnStopRoll(HalfHeightAdjust);
    }
    else
    {
        UpdatedComponent->MoveComponent(FVector(0.f, 0.f, ScaledHalfHeightAdjust), UpdatedComponent->GetComponentQuat(), true, nullptr,
            MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
        BaseCharacterOwner->OnStopRoll(0.0f);
    }
    SetMovementMode(MOVE_Walking);
}

void URSCharacterMovementComponent::AttachToLadder(const ALadder* Ladder)
{
    bOrientRotationToMovement = false;
    CurrentLadder = Ladder;
    FRotator TargetOrientationRotation = CurrentLadder->GetActorForwardVector().ToOrientationRotator();
    TargetOrientationRotation.Yaw += 180.0f;

    FVector LadderUpVector = CurrentLadder->GetActorUpVector();
    FVector LadderForwardVector = CurrentLadder->GetActorForwardVector();
    float Projection = GetActorToCurrentLadderProjection(GetActorLocation());

    FVector NewCharacterLocation = CurrentLadder->GetActorLocation() + Projection * LadderUpVector + LadderToCharacterOffset * LadderForwardVector;
    if(CurrentLadder->GetIsOnTop())
    {
        AttachToLadderFromTop();
    }
    else
    {
        GetOwner()->SetActorLocation(NewCharacterLocation);
        GetOwner()->SetActorRotation(TargetOrientationRotation);
        SetMovementMode(MOVE_Custom, StaticCast<uint8>(ECustomMovementMode::CMOVE_OnLadder));
    }
}

void URSCharacterMovementComponent::AttachToLadderFromTop()
{
    FRotator TargetOrientationRotation = CurrentLadder->GetActorForwardVector().ToOrientationRotator();
    FVector StartPos = CurrentLadder->GetActorLocation() + CurrentLadder->GetAttachFromTopStartPosition();
    GetOwner()->SetActorLocation(StartPos);
    ARSBaseCharacter* BaseCharacter = Cast<ARSBaseCharacter>(GetOwner());
    GetOwner()->SetActorRotation(TargetOrientationRotation);
    float MontageDuration = BaseCharacter->PlayAnimMontage(CurrentLadder->GetAttachFromTopAnimMontage());
    bIsAttachingToLadder = true;
    SetMovementMode(MOVE_Custom, StaticCast<uint8>(ECustomMovementMode::CMOVE_AttachingOnLadder));
    GetWorld()->GetTimerManager().SetTimer(LadderTimer, this, &URSCharacterMovementComponent::SetLadderMovement, MontageDuration - 1.0f, false);
}

void URSCharacterMovementComponent::DetachFromLadder(EDetachFromLadderMethod DetachFromLadderMethod)
{
    bOrientRotationToMovement = true;
    switch (DetachFromLadderMethod)
    {
        case EDetachFromLadderMethod::JumpOff:
        {
            FVector JumpDirection = CurrentLadder->GetActorForwardVector();
            SetMovementMode(MOVE_Falling);
            FVector JumpVelocity = JumpDirection * JumpOffFromLadderSpeed;
            Launch(JumpVelocity);
            break;
        }
        case EDetachFromLadderMethod::ReachingTheTop:
        {
            bIsAttachingToLadder = true;
            float MontageDuration = BaseCharacterOwner->PlayAnimMontage(CurrentLadder->GetAttachOnTopAnimMontage());
            GetWorld()->GetTimerManager().SetTimer(LadderTimer, this, &URSCharacterMovementComponent::SetWalkingMovement, MontageDuration, false);
            SetMovementMode(MOVE_Custom, StaticCast<uint8>(ECustomMovementMode::CMOVE_AttachingOnTopLadder));
            break;
        }
        case EDetachFromLadderMethod::ReachingTheBottom:
        {
            SetMovementMode(MOVE_Walking);
            break;
        }
        case EDetachFromLadderMethod::Fall:
            default:
        {
            SetMovementMode(MOVE_Falling);
            break;
        }
    }
}

void URSCharacterMovementComponent::SetLadderMovement()
{
    SetMovementMode(MOVE_Custom, StaticCast<uint8>(ECustomMovementMode::CMOVE_OnLadder));
    bIsAttachingToLadder = false;
}

void URSCharacterMovementComponent::SetWalkingMovement()
{
    SetMovementMode(MOVE_Walking);
    bIsAttachingToLadder = false;
}

bool URSCharacterMovementComponent::IsOnLadder() const
{
    return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == StaticCast<uint8>(ECustomMovementMode::CMOVE_OnLadder);
}

bool URSCharacterMovementComponent::IsOnWall() const
{
    return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == StaticCast<uint8>(ECustomMovementMode::CMOVE_OnWall);
}

void URSCharacterMovementComponent::AttachToWall(const AClimbingWall* Wall)
{
    CurrentWall = Wall;
    bOrientRotationToMovement = false;
    StopMovementImmediately();
    SetMovementMode(MOVE_Custom, StaticCast<uint8>(ECustomMovementMode::CMOVE_OnWall));
}

void URSCharacterMovementComponent::DetachFromWall()
{
    SetMovementMode(MOVE_Walking);
    BaseCharacterOwner->DetachFromWall();
}

void URSCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

    if (MovementMode == MOVE_Custom)
    {
        Velocity = FVector::ZeroVector;
        switch (CustomMovementMode)
        {
            case ECustomMovementMode::CMOVE_Mantling:
            {
                GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &URSCharacterMovementComponent::EndMantle,
                    CurrentMantlingParameters.Duration, false);
                break;
            }
            case ECustomMovementMode::CMOVE_Rolling:
            {
                GetWorld()->GetTimerManager().SetTimer(RollingTimer, this, &URSCharacterMovementComponent::StopRoll, CurrentRollingParameters.Duration , false);
                break;
            }
            default: break;
        }
    }
}

void URSCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
    switch (CustomMovementMode)
    {
        case ECustomMovementMode::CMOVE_Mantling:
        {
            PhysMantle(deltaTime, Iterations);
            break;
        }
        case ECustomMovementMode::CMOVE_Rolling:
        {
            PhysRolling(deltaTime, Iterations);
            break;
        }
        case ECustomMovementMode::CMOVE_OnLadder:
        {
            PhysLadder(deltaTime, Iterations);
            break;
        }
        case ECustomMovementMode::CMOVE_AttachingOnLadder:
        {
            PhysAttachToLadder(deltaTime, Iterations);
            break;
        }
        case ECustomMovementMode::CMOVE_AttachingOnTopLadder:
        {
            PhysAttachOnTopLadder(deltaTime, Iterations);
            break;
        }
        case ECustomMovementMode::CMOVE_OnRope:
        {
            PhysHanging(deltaTime, Iterations);
            break;
        }
        case ECustomMovementMode::CMOVE_OnWall:
        {
            PhysOnWall(deltaTime, Iterations);
        }
        default:
        {
            break;
        }
    }
    Super::PhysCustom(deltaTime, Iterations);
}

void URSCharacterMovementComponent::PhysMantle(float DeltaTime, int32 Iterations)
{
    const float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) + CurrentMantlingParameters.StartTime;

    const FVector CurveValue = CurrentMantlingParameters.MantlingCurve->GetVectorValue(ElapsedTime);
    const float PositionAlpha = CurveValue.X;
    const float XYCorrectionAlpha = CurveValue.Y;
    const float ZCorrectionAlpha = CurveValue.Z;

    FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingParameters.InitialLocation,
        CurrentMantlingParameters.InitialAnimationLocation, XYCorrectionAlpha);
    CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingParameters.InitialLocation.Z,
        CurrentMantlingParameters.InitialAnimationLocation.Z, ZCorrectionAlpha);

    const FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, CurrentMantlingParameters.TargetLocation, PositionAlpha);
    const FRotator NewRotation = FMath::Lerp(CurrentMantlingParameters.InitialRotation, CurrentMantlingParameters.TargetRotation,
        PositionAlpha);
    const FVector Delta = NewLocation - GetActorLocation();

    const FVector TargetDelta = Delta + (CurrentMantlingParameters.PrimitiveComponent->GetComponentLocation() -
        CurrentMantlingParameters.PrimitiveComponentInitialLocation);

    Velocity = Delta / DeltaTime;

    FHitResult HitResult;
    SafeMoveUpdatedComponent(TargetDelta, NewRotation, false, HitResult);
}

void URSCharacterMovementComponent::PhysRolling(float DeltaTime, int32 Iterations)
{
    const float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(RollingTimer);
    Velocity = BaseCharacterOwner->GetActorForwardVector() * CurrentRollingParameters.RollingCurve->GetFloatValue(ElapsedTime);
    
    FVector StandingLocation = UpdatedComponent->GetComponentLocation();
    StandingLocation.Z -= 2.0f;
    float DefaultHalfHeight = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
    float DefaultCapsuleRadius = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
    const ECollisionChannel CollisionChannel = UpdatedComponent->GetCollisionObjectType();
    const FCollisionShape StandingCapsuleShape = FCollisionShape::MakeCapsule(DefaultCapsuleRadius, DefaultHalfHeight);
    FCollisionQueryParams CapsuleParams;
    CapsuleParams.AddIgnoredActor(CharacterOwner);
    FCollisionResponseParams ResponseParams;
    InitCollisionParams(CapsuleParams, ResponseParams);
    bool bIsEnough = false;
    bIsEnough = GetWorld()->OverlapBlockingTestByChannel(StandingLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape,
        CapsuleParams, ResponseParams);
    if (!bIsEnough)
    {
        Velocity += GetGravityZ() * FVector::UpVector;
    }

    FVector Delta = Velocity * DeltaTime;
    Velocity = Delta / DeltaTime;
    FHitResult Hit;
    SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, Hit);
}

void URSCharacterMovementComponent::PhysAttachToLadder(float DeltaTime, int32 Iterations)
{
    const float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(LadderTimer);
    const FVector NewLocation = FMath::Lerp(GetOwner()->GetActorLocation(), CurrentLadder->GetAttachFromTopEndPosition(), CurrentLadder->GetAttachFromTopCurve()->GetFloatValue(ElapsedTime));
    const FVector Delta = NewLocation - GetActorLocation();

    Velocity = Delta / DeltaTime;

    FHitResult HitResult;
    SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), false, HitResult);
}

void URSCharacterMovementComponent::PhysAttachOnTopLadder(float DeltaTime, int32 Iterations)
{
    const float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(LadderTimer);
    const FVector NewLocation = FMath::Lerp(GetOwner()->GetActorLocation(), CurrentLadder->GetTopPosition(), CurrentLadder->GetAttachOnTopCurve()->GetFloatValue(ElapsedTime));
    const FVector Delta = NewLocation - GetActorLocation();

    Velocity = Delta / DeltaTime;

    FHitResult HitResult;
    SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), false, HitResult);
}


void URSCharacterMovementComponent::PhysLadder(float DeltaTime, int32 Iterations)
{
    CalcVelocity(DeltaTime, 1.0f, false, ClimbingOnLadderBrakingDecelaration);
    FVector Delta = Velocity * DeltaTime;

    if(HasAnimRootMotion())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), false, Hit);
        return;
    }
	
    FVector NewPos = GetActorLocation() + Delta;
    float NewPosProjection = GetActorToCurrentLadderProjection(NewPos);

    if(NewPosProjection < MinLadderBotomOffset)
    {
        DetachFromLadder(EDetachFromLadderMethod::ReachingTheBottom);
        return;
    }
    if(NewPosProjection > (CurrentLadder->GetLadderHeight() - MaxLadderTopOffset))
    {
        DetachFromLadder(EDetachFromLadderMethod::ReachingTheTop);
        return;
    }

    FHitResult Hit;
    SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, Hit);
}

void URSCharacterMovementComponent::PhysHanging(float DeltaTime, int32 Iterations)
{
    const ARope* CurrentRope = BaseCharacterOwner->GetAvailableRope();

    const FVector NewLocation = FMath::Lerp(GetOwner()->GetActorLocation(), CurrentRope->GetCableEndMeshComponent()->GetComponentLocation(), 0.2f);
    const FVector Delta = NewLocation - GetActorLocation();

    Velocity = Delta / DeltaTime;

    FHitResult HitResult;
    SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), false, HitResult);
}

void URSCharacterMovementComponent::PhysOnWall(float DeltaTime, int32 Iterations)
{
    CalcVelocity(DeltaTime, 1.0f, false, ClimbingOnLadderBrakingDecelaration);
    DetectWall();

    if(IsValid(CurrentWall))
    {
        FVector Delta = Velocity * DeltaTime;
        FVector NewPos = GetActorLocation() + Delta;
        float NewPosProjectionUpDown = GetActorToCurrentWallProjectionUpDown(NewPos);
        if(NewPosProjectionUpDown < MinWallBotomOffset)
        {
            Velocity = FVector::ZeroVector;
            return;
        }
        if(NewPosProjectionUpDown > (CurrentWall->GetWallLength() - MaxWallTopOffset))
        {
            FLedgeDescription LedgeDescription;
            if(BaseCharacterOwner->GetLedgeDetectorComponent()->LedgeDetect(LedgeDescription))
            {
                BaseCharacterOwner->Mantle(true);
                BaseCharacterOwner->DetachFromWall();
            }
            else
            {
                Velocity = FVector::ZeroVector;
                return;
            }
        }
        FVector CurrentWallForwardVector = -CurrentHitWall.ImpactNormal;
        FRotator NextRotation = UKismetMathLibrary::RInterpTo(CharacterOwner->GetActorRotation(), CurrentWallForwardVector.ToOrientationRotator(), DeltaTime, CurrentWall->GetInterpSpeed());
        CharacterOwner->SetActorRotation(NextRotation, ETeleportType::TeleportPhysics);
        FHitResult Hit;
        SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, Hit);
    }
}

bool URSCharacterMovementComponent::IsEnoughSpaceToStandUp()
{
    FVector StandingLocation = UpdatedComponent->GetComponentLocation();
    ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
    float DefaultHalfHeight = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
    float DefaultCapsuleRadius = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
    float Offset = 30.0f;
    FVector LocationWithOffset = StandingLocation + Offset * FVector::UpVector;
    const ECollisionChannel CollisionChannel = UpdatedComponent->GetCollisionObjectType();
    const FCollisionShape StandingCapsuleShape = FCollisionShape::MakeCapsule(DefaultCapsuleRadius, DefaultHalfHeight);
    FCollisionQueryParams CapsuleParams;
    CapsuleParams.AddIgnoredActor(CharacterOwner);
    FCollisionResponseParams ResponseParams;
    InitCollisionParams(CapsuleParams, ResponseParams);
    bool bIsEnough = false;
    bIsEnough = GetWorld()->OverlapBlockingTestByChannel(LocationWithOffset + Offset * FVector::UpVector, FQuat::Identity, CollisionChannel,
        StandingCapsuleShape, CapsuleParams, ResponseParams);
    return bIsEnough;
}

float URSCharacterMovementComponent::GetActorToCurrentLadderProjection(FVector ActorLocation)
{
    checkf(IsValid(CurrentLadder), TEXT("URSCharacterMovementComponent::GetCharacterToCurrentLadderProjection() can not be invoked when current ladder is null"));
    FVector LadderUpVector = CurrentLadder->GetActorUpVector();
    FVector LadderToCharacterDistance = ActorLocation - CurrentLadder->GetActorLocation();
    return FVector::DotProduct(LadderUpVector, LadderToCharacterDistance);
}

float URSCharacterMovementComponent::GetActorToCurrentWallProjectionUpDown(FVector ActorLocation)
{
    checkf(IsValid(CurrentWall), TEXT("URSCharacterMovementComponent::GetCharacterToCurrentWallProjection() can not be invoked when current wall is null"));
    FVector WallUpVector = CurrentWall->GetActorUpVector();
    FVector WallToCharacterDistance = ActorLocation - CurrentWall->GetActorLocation();
    return FVector::DotProduct(WallUpVector, WallToCharacterDistance);
}

float URSCharacterMovementComponent::GetActorToCurrentWallProjectionLeftRight(FVector ActorLocation)
{
    checkf(IsValid(CurrentWall), TEXT("URSCharacterMovementComponent::GetCharacterToCurrentWallProjection() can not be invoked when current wall is null"));
    FVector WallRightVector = CurrentWall->GetActorRightVector();
    FVector WallToCharacterDistance = ActorLocation - CurrentWall->GetActorLocation();
    return FVector::DotProduct(WallRightVector, WallToCharacterDistance);
}

void URSCharacterMovementComponent::DetectWall()
{
    FVector StartPosition = GetActorLocation();
    FVector EndPosition =  StartPosition + 100.0f * BaseCharacterOwner->GetActorForwardVector();
    FHitResult TraceHit;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(CharacterOwner);
    if(GetWorld()->LineTraceSingleByChannel(TraceHit, StartPosition, EndPosition, ECC_GameTraceChannel3, QueryParams, FCollisionResponseParams::DefaultResponseParam))
    {
        CurrentWall = Cast<AClimbingWall>(TraceHit.Actor);
        if(GetWorld()->LineTraceSingleByChannel(TraceHit, StartPosition, EndPosition, ECC_GameTraceChannel4, QueryParams, FCollisionResponseParams::DefaultResponseParam))
        {
            CurrentHitWall = TraceHit;
        }
        else
        {
            DetachFromWall();
        }
    }
    else
    {
        DetachFromWall();
    }
}
