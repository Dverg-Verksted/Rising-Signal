// It is owned by the company Dverg Verksted.


#include "Player/RSCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "NewTestPlayer/RSBaseCharacter.h"

void URSCharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay();
    BaseCharacterOwner = StaticCast<ARSBaseCharacter*>(GetOwner());
}

void URSCharacterMovementComponent::StartMantle(const FMantlingMovementParameters& MantlingMovementParameters)
{
    CurrentMantlingParameters = MantlingMovementParameters;
    SetMovementMode(MOVE_Custom, StaticCast<uint8>(ECustomMovementMode::CMOVE_Mantling));
}

void URSCharacterMovementComponent::EndMantle()
{
    BaseCharacterOwner->SetIsMantling(false);
    SetMovementMode(MOVE_Walking);
}

void URSCharacterMovementComponent::StartRoll()
{
    /*const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();
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

    BaseCharacterOwner->OnStartRoll(HalfHeightAdjust);
    SetMovementMode(MOVE_Custom, StaticCast<uint8>(ECustomMovementMode::CMOVE_Rolling));*/
}

void URSCharacterMovementComponent::StopRoll()
{
    /*const ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
    const float DefaultHalfHeight = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
    const float HalfHeightAdjust = DefaultHalfHeight - CrouchedHalfHeight;
    CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
    BaseCharacterOwner->SetIsRolling(false);
    SetMovementMode(MOVE_Walking);
    if (IsEnoughSpaceToStandUp())
    {
        Crouch();
        BaseCharacterOwner->OnEndRoll(HalfHeightAdjust);
    }
    else
    {
        BaseCharacterOwner->OnEndRoll(0.0f);
    }*/
}

void URSCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

    if (MovementMode == MOVE_Custom)
    {
        Velocity = FVector::ZeroVector;
        switch (CustomMovementMode)
        {
            case (uint8)ECustomMovementMode::CMOVE_Mantling:
            {
                GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &URSCharacterMovementComponent::EndMantle,
                    CurrentMantlingParameters.Duration, false);
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
    /*Velocity = BaseCharacterOwner->GetActorForwardVector() * RollSpeed;

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
    SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, Hit);*/
}

bool URSCharacterMovementComponent::IsEnoughSpaceToStandUp()
{
    /*FVector StandingLocation = UpdatedComponent->GetComponentLocation();
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
    return bIsEnough;*/
    return false;
}
