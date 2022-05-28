// Copyright (C) 2020, Doga Can Yanikoglu

#include "Character/ALSBaseCharacter.h"

#include "Engine.h"
#include "Character/ALSPlayerController.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

AALSBaseCharacter::AALSBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	MantleTimeline = CreateDefaultSubobject<UTimelineComponent>(FName(TEXT("MantleTimeline")));
	bUseControllerRotationYaw = 0;
}

void AALSBaseCharacter::Restart()
{
	Super::Restart();

	AALSPlayerController* NewController = Cast<AALSPlayerController>(GetController());
	if (NewController)
	{
		NewController->OnRestartPawn(this);
	}
}

void AALSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward/Backwards", this, &AALSBaseCharacter::PlayerForwardMovementInput);
	PlayerInputComponent->BindAxis("MoveRight/Left", this, &AALSBaseCharacter::PlayerRightMovementInput);
	// PlayerInputComponent->BindAxis("LookUp/Down", this, &AALSBaseCharacter::PlayerCameraUpInput);
	// PlayerInputComponent->BindAxis("LookLeft/Right", this, &AALSBaseCharacter::PlayerCameraRightInput);
	PlayerInputComponent->BindAction("JumpAction", IE_Pressed, this, &AALSBaseCharacter::JumpPressedAction);
	PlayerInputComponent->BindAction("JumpAction", IE_Released, this, &AALSBaseCharacter::JumpReleasedAction);
	// PlayerInputComponent->BindAction("StanceAction", IE_Pressed, this, &AALSBaseCharacter::StancePressedAction);
	// PlayerInputComponent->BindAction("WalkAction", IE_Pressed, this, &AALSBaseCharacter::WalkPressedAction);
	// PlayerInputComponent->BindAction("RagdollAction", IE_Pressed, this, &AALSBaseCharacter::RagdollPressedAction);
	// PlayerInputComponent->BindAction("SelectRotationMode_1", IE_Pressed, this, &AALSBaseCharacter::VelocityDirectionPressedAction);
	// PlayerInputComponent->BindAction("SelectRotationMode_2", IE_Pressed, this, &AALSBaseCharacter::LookingDirectionPressedAction);
	PlayerInputComponent->BindAction("SprintAction", IE_Pressed, this, &AALSBaseCharacter::SprintPressedAction);
	PlayerInputComponent->BindAction("SprintAction", IE_Released, this, &AALSBaseCharacter::SprintReleasedAction);
	// PlayerInputComponent->BindAction("AimAction", IE_Pressed, this, &AALSBaseCharacter::AimPressedAction);
	// PlayerInputComponent->BindAction("AimAction", IE_Released, this, &AALSBaseCharacter::AimReleasedAction);
	// PlayerInputComponent->BindAction("CameraAction", IE_Pressed, this, &AALSBaseCharacter::CameraPressedAction);
	// PlayerInputComponent->BindAction("CameraAction", IE_Released, this, &AALSBaseCharacter::CameraReleasedAction);
}

void AALSBaseCharacter::OnBreakfall_Implementation()
{
	MainAnimInstance->Montage_Play(GetRollAnimation(), 1.35f);
}

void AALSBaseCharacter::OnRoll_Implementation()
{
	// Roll: Simply play a Root Motion Montage.
	MainAnimInstance->Montage_Play(GetRollAnimation(), 1.15f);
}

void AALSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat TimelineUpdated;
	FOnTimelineEvent TimelineFinished;
	TimelineUpdated.BindUFunction(this, FName(TEXT("MantleUpdate")));
	TimelineFinished.BindUFunction(this, FName(TEXT("MantleEnd")));
	MantleTimeline->SetTimelineFinishedFunc(TimelineFinished);
	MantleTimeline->SetLooping(false);
	MantleTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	MantleTimeline->AddInterpFloat(MantleTimelineCurve, TimelineUpdated);

	// Make sure the mesh and animbp update after the CharacterBP to ensure it gets the most recent values.
	GetMesh()->AddTickPrerequisiteActor(this);

	// Set the Movement Model
	SetMovementModel();

	// Once, force set variables in anim bp. This ensures anim instance & character starts synchronized
	FALSAnimCharacterInformation& AnimData = MainAnimInstance->GetCharacterInformationMutable();
	AnimData.Gait = DesiredGait;
	AnimData.RotationMode = DesiredRotationMode;
	AnimData.ViewMode = ViewMode;
	AnimData.OverlayState = OverlayState;
	AnimData.PrevMovementState = PrevMovementState;
	AnimData.MovementState = MovementState;

	// Update states to use the initial desired values.
	SetGait(DesiredGait);
	SetRotationMode(DesiredRotationMode);
	SetViewMode(ViewMode);
	SetOverlayState(OverlayState);

	if (Stance == EALSStance::Standing)
	{
		UnCrouch();
	}
	else if (Stance == EALSStance::Crouching)
	{
		Crouch();
	}

	// Set default rotation values.
	TargetRotation = GetActorRotation();
	LastVelocityRotation = TargetRotation;
	LastMovementInputRotation = TargetRotation;
}

void AALSBaseCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	MainAnimInstance = Cast<UALSCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	// TODO: Check null for MainAnimInstance if that's not editor object
}

void AALSBaseCharacter::SetAimYawRate(float NewAimYawRate)
{
	AimYawRate = NewAimYawRate;
	MainAnimInstance->GetCharacterInformationMutable().AimYawRate = AimYawRate;
}

void AALSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set required values
	SetEssentialValues(DeltaTime);

	// Cache values
	PreviousVelocity = GetVelocity();
	PreviousAimYaw = GetControlRotation().Yaw;

	if (MovementState == EALSMovementState::Grounded)
	{
		UpdateCharacterMovement();
		UpdateGroundedRotation(DeltaTime);
	}
	else if (MovementState == EALSMovementState::InAir)
	{
		UpdateInAirRotation(DeltaTime);

		// Perform a mantle check if falling while movement input is pressed.
		if (bHasMovementInput)
		{
			MantleCheck(FallingTraceSettings);
		}
	}
	else if (MovementState == EALSMovementState::Ragdoll)
	{
		RagdollUpdate();
	}

	DrawDebugSpheres();
}

void AALSBaseCharacter::RagdollStart()
{
	// Step 1: Clear the Character Movement Mode and set teh Movement State to Ragdoll
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetMovementState(EALSMovementState::Ragdoll);

	// Step 2: Disable capsule collision and enable mesh physics simulation starting from the pelvis.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(FName(TEXT("Pelvis")), true, true);

	// Step 3: Stop any active montages.
	MainAnimInstance->Montage_Stop(0.2f);
}

void AALSBaseCharacter::RagdollEnd()
{
	if (!MainAnimInstance)
	{
		return;
	}

	// Step 1: Save a snapshot of the current Ragdoll Pose for use in AnimGraph to blend out of the ragdoll
	MainAnimInstance->SavePoseSnapshot(FName(TEXT("RagdollPose")));

	// Step 2: If the ragdoll is on the ground, set the movement mode to walking and play a Get Up animation.
	// If not, set the movement mode to falling and update teh character movement velocity to match the last ragdoll velocity.
	if (bRagdollOnGround)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		MainAnimInstance->Montage_Play(GetGetUpAnimation(bRagdollFaceUp), 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		GetCharacterMovement()->Velocity = LastRagdollVelocity;
	}

	// Step 3: Re-Enable capsule collision, and disable physics simulation on the mesh.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetAllBodiesSimulatePhysics(false);
}

void AALSBaseCharacter::SetMovementState(const EALSMovementState NewState)
{
	if (MovementState != NewState)
	{
		PrevMovementState = MovementState;
		MovementState = NewState;
		FALSAnimCharacterInformation& AnimData = MainAnimInstance->GetCharacterInformationMutable();
		AnimData.PrevMovementState = PrevMovementState;
		AnimData.MovementState = MovementState;
		OnMovementStateChanged(PrevMovementState);
	}
}

void AALSBaseCharacter::SetMovementAction(const EALSMovementAction NewAction)
{
	if (MovementAction != NewAction)
	{
		EALSMovementAction Prev = MovementAction;
		MovementAction = NewAction;
		MainAnimInstance->GetCharacterInformationMutable().MovementAction = MovementAction;
		OnMovementActionChanged(Prev);
	}
}

void AALSBaseCharacter::SetStance(const EALSStance NewStance)
{
	if (Stance != NewStance)
	{
		EALSStance Prev = Stance;
		Stance = NewStance;
		MainAnimInstance->GetCharacterInformationMutable().Stance = Stance;
		OnStanceChanged(Prev);
	}
}

void AALSBaseCharacter::SetRotationMode(const EALSRotationMode NewRotationMode)
{
	if (RotationMode != NewRotationMode)
	{
		EALSRotationMode Prev = RotationMode;
		RotationMode = NewRotationMode;
		MainAnimInstance->GetCharacterInformationMutable().RotationMode = RotationMode;
		OnRotationModeChanged(Prev);
	}
}

void AALSBaseCharacter::SetGait(const EALSGait NewGait)
{
	if (Gait != NewGait)
	{
		EALSGait Prev = Gait;
		Gait = NewGait;
		MainAnimInstance->GetCharacterInformationMutable().Gait = Gait;
		OnGaitChanged(Prev);
	}
}

void AALSBaseCharacter::SetViewMode(const EALSViewMode NewViewMode)
{
	if (ViewMode != NewViewMode)
	{
		EALSViewMode Prev = ViewMode;
		ViewMode = NewViewMode;
		MainAnimInstance->GetCharacterInformationMutable().ViewMode = ViewMode;
		OnViewModeChanged(Prev);
	}
}

void AALSBaseCharacter::SetOverlayState(const EALSOverlayState NewState)
{
	if (OverlayState != NewState)
	{
		EALSOverlayState Prev = OverlayState;
		OverlayState = NewState;
		MainAnimInstance->GetCharacterInformationMutable().OverlayState = OverlayState;
		OnOverlayStateChanged(Prev);
	}
}

void AALSBaseCharacter::SetActorLocationAndTargetRotation(FVector NewLocation, FRotator NewRotation)
{
	SetActorLocationAndRotation(NewLocation, NewRotation);
	TargetRotation = NewRotation;
}

bool AALSBaseCharacter::MantleCheckGrounded()
{
	return MantleCheck(GroundedTraceSettings);
}

bool AALSBaseCharacter::MantleCheckFalling()
{
	return MantleCheck(FallingTraceSettings);
}

void AALSBaseCharacter::SetMovementModel()
{
	FString ContextString = GetFullName();
	FALSMovementStateSettings* OutRow = MovementModel.DataTable->FindRow<FALSMovementStateSettings>(MovementModel.RowName, ContextString);
	check(OutRow);
	MovementData = *OutRow;
}

void AALSBaseCharacter::SetHasMovementInput(bool bNewHasMovementInput)
{
	bHasMovementInput = bNewHasMovementInput;
	MainAnimInstance->GetCharacterInformationMutable().bHasMovementInput = bHasMovementInput;
}

FALSMovementSettings AALSBaseCharacter::GetTargetMovementSettings()
{
	if (RotationMode == EALSRotationMode::VelocityDirection)
	{
		if (Stance == EALSStance::Standing)
		{
			return MovementData.VelocityDirection.Standing;
		}
		if (Stance == EALSStance::Crouching)
		{
			return MovementData.VelocityDirection.Crouching;
		}
	}
	else if (RotationMode == EALSRotationMode::LookingDirection)
	{
		if (Stance == EALSStance::Standing)
		{
			return MovementData.LookingDirection.Standing;
		}
		if (Stance == EALSStance::Crouching)
		{
			return MovementData.LookingDirection.Crouching;
		}
	}
	else if (RotationMode == EALSRotationMode::Aiming)
	{
		if (Stance == EALSStance::Standing)
		{
			return MovementData.Aiming.Standing;
		}
		if (Stance == EALSStance::Crouching)
		{
			return MovementData.Aiming.Crouching;
		}
	}

	// Default to velocity dir standing
	return MovementData.VelocityDirection.Standing;
}

bool AALSBaseCharacter::CanSprint()
{
	// Determine if the character is currently able to sprint based on the Rotation mode and current acceleration
	// (input) rotation. If the character is in the Looking Rotation mode, only allow sprinting if there is full
	// movement input and it is faced forward relative to the camera + or - 50 degrees.

	if (!bHasMovementInput || RotationMode == EALSRotationMode::Aiming)
	{
		return false;
	}

	const bool bValidInputAmount = MovementInputAmount > 0.9f;

	if (RotationMode == EALSRotationMode::VelocityDirection)
	{
		return bValidInputAmount;
	}

	if (RotationMode == EALSRotationMode::LookingDirection)
	{
		const FRotator AccRot = GetCharacterMovement()->GetCurrentAcceleration().ToOrientationRotator();
		FRotator Delta = AccRot - GetControlRotation();
		Delta.Normalize();

		return bValidInputAmount && FMath::Abs(Delta.Yaw) < 50.0f;
	}

	return false;
}

void AALSBaseCharacter::SetIsMoving(bool bNewIsMoving)
{
	bIsMoving = bNewIsMoving;
	MainAnimInstance->GetCharacterInformationMutable().bIsMoving = bIsMoving;
}

FVector AALSBaseCharacter::GetMovementInput()
{
	return GetCharacterMovement()->GetCurrentAcceleration();
}

void AALSBaseCharacter::SetMovementInputAmount(float NewMovementInputAmount)
{
	MovementInputAmount = NewMovementInputAmount;
	MainAnimInstance->GetCharacterInformationMutable().MovementInputAmount = MovementInputAmount;
}

void AALSBaseCharacter::SetSpeed(float NewSpeed)
{
	Speed = NewSpeed;
	MainAnimInstance->GetCharacterInformationMutable().Speed = Speed;
}

float AALSBaseCharacter::GetAnimCurveValue(FName CurveName)
{
	if (MainAnimInstance)
	{
		return MainAnimInstance->GetCurveValue(CurveName);
	}

	return 0.0f;
}

ECollisionChannel AALSBaseCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	TraceOrigin = GetActorLocation();
	TraceRadius = 10.0f;
	return ECC_Visibility;
}

FTransform AALSBaseCharacter::GetThirdPersonPivotTarget()
{
	return GetActorTransform();
}

FVector AALSBaseCharacter::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(FName(TEXT("FP_Camera")));
}

void AALSBaseCharacter::GetCameraParameters(float& TPFOVOut, float& FPFOVOut, bool& bRightShoulderOut)
{
	TPFOVOut = ThirdPersonFOV;
	FPFOVOut = FirstPersonFOV;
	bRightShoulderOut = bRightShoulder;
}

void AALSBaseCharacter::SetAcceleration(const FVector& NewAcceleration)
{
	Acceleration = NewAcceleration;
	MainAnimInstance->GetCharacterInformationMutable().Acceleration = Acceleration;
}

void AALSBaseCharacter::RagdollUpdate()
{
	// Set the Last Ragdoll Velocity.
	LastRagdollVelocity = GetMesh()->GetPhysicsLinearVelocity(FName(TEXT("Root")));

	// Use the Ragdoll Velocity to scale the ragdoll's joint strength for physical animation.
	const float SpringValue =
		FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1000.0f), FVector2D(0.0f, 25000.0f), LastRagdollVelocity.Size());
	GetMesh()->SetAllMotorsAngularDriveParams(SpringValue, 0.0f, 0.0f, false);

	// Disable Gravity if falling faster than -4000 to prevent continual acceleration.
	// This also prevents the ragdoll from going through the floor.
	const bool bEnableGrav = LastRagdollVelocity.Z > -4000.0f;
	GetMesh()->SetEnableGravity(bEnableGrav);

	// Update the Actor location to follow the ragdoll.
	SetActorLocationDuringRagdoll();
}

void AALSBaseCharacter::SetActorLocationDuringRagdoll()
{
	// Set the pelvis as the target location.
	const FVector TargetRagdollLocation = GetMesh()->GetSocketLocation(FName(TEXT("Pelvis")));

	// Determine wether the ragdoll is facing up or down and set the target rotation accordingly.
	const FRotator PelvisRot = GetMesh()->GetSocketRotation(FName(TEXT("Pelvis")));

	bRagdollFaceUp = PelvisRot.Roll < 0.0f;

	const FRotator TargetRagdollRotation = FRotator(0.0f, bRagdollFaceUp ? PelvisRot.Yaw - 180.0f : PelvisRot.Yaw, 0.0f);

	// Trace downward from the target location to offset the target location,
	// preventing the lower half of the capsule from going through the floor when the ragdoll is laying on the ground.
	const FVector TraceVect(
		TargetRagdollLocation.X, TargetRagdollLocation.Y, TargetRagdollLocation.Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TargetRagdollLocation, TraceVect, ECC_Visibility, Params);

	bRagdollOnGround = HitResult.IsValidBlockingHit();
	if (bRagdollOnGround)
	{
		const float ImpactDistZ = FMath::Abs(HitResult.ImpactPoint.Z - HitResult.TraceStart.Z);
		FVector NewRagdollLoc = TargetRagdollLocation;
		NewRagdollLoc.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - ImpactDistZ + 2.0f;
		SetActorLocationAndTargetRotation(NewRagdollLoc, TargetRagdollRotation);
	}
	else
	{
		SetActorLocationAndTargetRotation(TargetRagdollLocation, TargetRagdollRotation);
	}
}

void AALSBaseCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	// Use the Character Movement Mode changes to set the Movement States to the right values. This allows you to have
	// a custom set of movement states but still use the functionality of the default character movement component.

	if (GetCharacterMovement()->MovementMode == MOVE_Walking || GetCharacterMovement()->MovementMode == MOVE_NavWalking)
	{
		SetMovementState(EALSMovementState::Grounded);
	}
	else if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		SetMovementState(EALSMovementState::InAir);
	}
}

void AALSBaseCharacter::OnMovementStateChanged(const EALSMovementState PreviousState)
{
	if (MovementState == EALSMovementState::InAir)
	{
		if (MovementAction == EALSMovementAction::None)
		{
			// If the character enters the air, set the In Air Rotation and uncrouch if crouched.
			InAirRotation = GetActorRotation();
			if (Stance == EALSStance::Crouching)
			{
				UnCrouch();
			}
		}
		else if (MovementAction == EALSMovementAction::Rolling)
		{
			// If the character is currently rolling, enable the ragdoll.
			RagdollStart();
		}
	}
	else if (MovementState == EALSMovementState::Ragdoll && PreviousState == EALSMovementState::Mantling)
	{
		// Stop the Mantle Timeline if transitioning to the ragdoll state while mantling.
		MantleTimeline->Stop();
	}
}

void AALSBaseCharacter::OnMovementActionChanged(const EALSMovementAction PreviousAction)
{
	// Make the character crouch if performing a roll.
	if (MovementAction == EALSMovementAction::Rolling)
	{
		Crouch();
	}

	if (PreviousAction == EALSMovementAction::Rolling)
	{
		if (DesiredStance == EALSStance::Standing)
		{
			UnCrouch();
		}
		else if (DesiredStance == EALSStance::Crouching)
		{
			Crouch();
		}
	}
}

void AALSBaseCharacter::OnStanceChanged(const EALSStance PreviousStance) {}

void AALSBaseCharacter::OnRotationModeChanged(EALSRotationMode PreviousRotationMode)
{
	if (RotationMode == EALSRotationMode::VelocityDirection && ViewMode == EALSViewMode::FirstPerson)
	{
		// If the new rotation mode is Velocity Direction and the character is in First Person,
		// set the viewmode to Third Person.
		SetViewMode(EALSViewMode::ThirdPerson);
	}
}

void AALSBaseCharacter::OnGaitChanged(const EALSGait PreviousGait) {}

void AALSBaseCharacter::OnViewModeChanged(const EALSViewMode PreviousViewMode)
{
	if (ViewMode == EALSViewMode::ThirdPerson)
	{
		if (RotationMode == EALSRotationMode::VelocityDirection || RotationMode == EALSRotationMode::LookingDirection)
		{
			// If Third Person, set the rotation mode back to the desired mode.
			SetRotationMode(DesiredRotationMode);
		}
	}
	else if (ViewMode == EALSViewMode::FirstPerson && RotationMode == EALSRotationMode::VelocityDirection)
	{
		// If First Person, set the rotation mode to looking direction if currently in the velocity direction mode.
		SetRotationMode(EALSRotationMode::LookingDirection);
	}
}

void AALSBaseCharacter::OnOverlayStateChanged(const EALSOverlayState PreviousState) {}

void AALSBaseCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	SetStance(EALSStance::Crouching);
}

void AALSBaseCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	SetStance(EALSStance::Standing);
}

void AALSBaseCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	// Set the new In Air Rotation to the velocity rotation if speed is greater than 100.
	InAirRotation = Speed > 100.0f ? LastVelocityRotation : GetActorRotation();
	MainAnimInstance->OnJumped();
}

void AALSBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	const float VelZ = FMath::Abs(GetCharacterMovement()->Velocity.Z);

	if (bHasMovementInput && VelZ >= 600.0f && VelZ <= 1000.0f)
	{
		OnBreakfall();
	}
	else if (VelZ > 1000.0f)
	{
		RagdollStart();
	}
	else
	{
		GetCharacterMovement()->BrakingFrictionFactor = bHasMovementInput ? 0.5f : 3.0f;

		// After 0.5 secs, reset braking friction factor to zero
		GetWorldTimerManager().SetTimer(OnLandedFrictionResetTimer, this, &AALSBaseCharacter::OnLandFrictionReset, 0.5f, false);
	}
}

void AALSBaseCharacter::OnLandFrictionReset()
{
	// Reset the braking friction
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
}

void AALSBaseCharacter::SetEssentialValues(float DeltaTime)
{
	// These values represent how the capsule is moving as well as how it wants to move, and therefore are essential
	// for any data driven animation system. They are also used throughout the system for various functions,
	// so I found it is easiest to manage them all in one place.

	const FVector CurrentVel = GetVelocity();

	// Set the amount of Acceleration.
	SetAcceleration((CurrentVel - PreviousVelocity) / DeltaTime);

	// Determine if the character is moving by getting it's speed. The Speed equals the length of the horizontal (x y)
	// velocity, so it does not take vertical movement into account. If the character is moving, update the last
	// velocity rotation. This value is saved because it might be useful to know the last orientation of movement
	// even after the character has stopped.
	SetSpeed(CurrentVel.Size2D());
	SetIsMoving(Speed > 1.0f);
	if (bIsMoving)
	{
		LastVelocityRotation = CurrentVel.ToOrientationRotator();
	}

	// Determine if the character has movement input by getting its movement input amount.
	// The Movement Input Amount is equal to the current acceleration divided by the max acceleration so that
	// it has a range of 0-1, 1 being the maximum possible amount of input, and 0 beiung none.
	// If the character has movement input, update the Last Movement Input Rotation.
	FVector CurAcc = GetCharacterMovement()->GetCurrentAcceleration();
	SetMovementInputAmount(CurAcc.Size() / GetCharacterMovement()->GetMaxAcceleration());
	SetHasMovementInput(MovementInputAmount > 0.0f);
	if (bHasMovementInput)
	{
		LastMovementInputRotation = CurAcc.ToOrientationRotator();
	}

	// Set the Aim Yaw rate by comparing the current and previous Aim Yaw value, divided by Delta Seconds.
	// This represents the speed the camera is rotating left to right.
	SetAimYawRate(FMath::Abs((GetControlRotation().Yaw - PreviousAimYaw) / DeltaTime));
}

void AALSBaseCharacter::UpdateCharacterMovement()
{
	// Set the Allowed Gait
	const EALSGait AllowedGait = GetAllowedGait();

	// Determine the Actual Gait. If it is different from the current Gait, Set the new Gait Event.
	const EALSGait ActualGait = GetActualGait(AllowedGait);

	if (ActualGait != Gait)
	{
		SetGait(ActualGait);
	}

	// Use the allowed gait to update the movement settings.
	UpdateDynamicMovementSettings(AllowedGait);
}

void AALSBaseCharacter::UpdateDynamicMovementSettings(EALSGait AllowedGait)
{
	// Get the Current Movement Settings.
	CurrentMovementSettings = GetTargetMovementSettings();

	// Update the Character Max Walk Speed to the configured speeds based on the currently Allowed Gait.
	GetCharacterMovement()->MaxWalkSpeed = CurrentMovementSettings.GetSpeedForGait(AllowedGait);
	GetCharacterMovement()->MaxWalkSpeedCrouched = GetCharacterMovement()->MaxWalkSpeed;

	// Update the Acceleration, Deceleration, and Ground Friction using the Movement Curve.
	// This allows for fine control over movement behavior at each speed (May not be suitable for replication).
	const float MappedSpeed = GetMappedSpeed();
	const FVector CurveVec = CurrentMovementSettings.MovementCurve->GetVectorValue(MappedSpeed);
	GetCharacterMovement()->MaxAcceleration = CurveVec.X;
	GetCharacterMovement()->BrakingDecelerationWalking = CurveVec.Y;
	GetCharacterMovement()->GroundFriction = CurveVec.Z;
}

void AALSBaseCharacter::UpdateGroundedRotation(float DeltaTime)
{
	if (MovementAction == EALSMovementAction::None)
	{
		const bool bCanUpdateMovingRot = ((bIsMoving && bHasMovementInput) || Speed > 150.0f) && !HasAnyRootMotion();
		if (bCanUpdateMovingRot)
		{
			const float GroundedRotationRate = CalculateGroundedRotationRate();
			if (RotationMode == EALSRotationMode::VelocityDirection)
			{
				// Velocity Direction Rotation
				SmoothCharacterRotation(FRotator(0.0f, LastVelocityRotation.Yaw, 0.0f), 800.0f, GroundedRotationRate, DeltaTime);
			}
			else if (RotationMode == EALSRotationMode::LookingDirection)
			{
				// Looking Direction Rotation
				float YawValue;
				if (Gait == EALSGait::Sprinting)
				{
					YawValue = LastVelocityRotation.Yaw;
				}
				else
				{
					// Walking or Running..
					const float YawOffsetCurveVal = MainAnimInstance->GetCurveValue(FName(TEXT("YawOffset")));
					YawValue = GetControlRotation().Yaw + YawOffsetCurveVal;
				}
				SmoothCharacterRotation(FRotator(0.0f, YawValue, 0.0f), 500.0f, GroundedRotationRate, DeltaTime);
			}
			else if (RotationMode == EALSRotationMode::Aiming)
			{
				const float ControlYaw = GetControlRotation().Yaw;
				SmoothCharacterRotation(FRotator(0.0f, ControlYaw, 0.0f), 1000.0f, 20.0f, DeltaTime);
			}
		}
		else
		{
			// Not Moving

			if (ViewMode == EALSViewMode::ThirdPerson && RotationMode == EALSRotationMode::Aiming || ViewMode == EALSViewMode::FirstPerson)
			{
				LimitRotation(-100.0f, 100.0f, 20.0f, DeltaTime);
			}

			// Apply the RotationAmount curve from Turn In Place Animations.
			// The Rotation Amount curve defines how much rotation should be applied each frame,
			// and is calculated for animations that are animated at 30fps.

			const float RotAmountCurve = MainAnimInstance->GetCurveValue(FName(TEXT("RotationAmount")));

			if (FMath::Abs(RotAmountCurve) > 0.001f)
			{
				AddActorWorldRotation(FRotator(0.0f, RotAmountCurve * (DeltaTime / (1.0f / 30.0f)), 0.0f));
				TargetRotation = GetActorRotation();
			}
		}
	}
	else if (MovementAction == EALSMovementAction::Rolling)
	{
		// Rolling Rotation

		if (bHasMovementInput)
		{
			SmoothCharacterRotation(FRotator(0.0f, LastMovementInputRotation.Yaw, 0.0f), 0.0f, 2.0f, DeltaTime);
		}
	}

	// Other actions are ignored...
}

void AALSBaseCharacter::UpdateInAirRotation(float DeltaTime)
{
	if (RotationMode == EALSRotationMode::VelocityDirection || RotationMode == EALSRotationMode::LookingDirection)
	{
		// Velocity / Looking Direction Rotation
		SmoothCharacterRotation(FRotator(0.0f, InAirRotation.Yaw, 0.0f), 0.0f, 5.0f, DeltaTime);
	}
	else if (RotationMode == EALSRotationMode::Aiming)
	{
		// Aiming Rotation
		SmoothCharacterRotation(FRotator(0.0f, GetControlRotation().Yaw, 0.0f), 0.0f, 15.0f, DeltaTime);
		InAirRotation = GetActorRotation();
	}
}

static FTransform TransfromSub(const FTransform& T1, const FTransform& T2)
{
	return FTransform(
		T1.GetRotation().Rotator() - T2.GetRotation().Rotator(), T1.GetLocation() - T2.GetLocation(), T1.GetScale3D() - T2.GetScale3D());
}

static FTransform TransfromAdd(const FTransform& T1, const FTransform& T2)
{
	return FTransform(
		T1.GetRotation().Rotator() + T2.GetRotation().Rotator(), T1.GetLocation() + T2.GetLocation(), T1.GetScale3D() + T2.GetScale3D());
}

static FVector GetCapsuleBaseLocation(const float ZOffset, UCapsuleComponent* Capsule)
{
	return Capsule->GetComponentLocation() - Capsule->GetUpVector() * (Capsule->GetScaledCapsuleHalfHeight() + ZOffset);
}

static FVector GetCapsuleLocationFromBase(FVector BaseLocation, const float ZOffset, UCapsuleComponent* Capsule)
{
	BaseLocation.Z += Capsule->GetScaledCapsuleHalfHeight() + ZOffset;
	return BaseLocation;
}

void AALSBaseCharacter::MantleStart(float MantleHeight, const FALSComponentAndTransform& MantleLedgeWS, EALSMantleType MantleType)
{
	// Step 1: Get the Mantle Asset and use it to set the new Mantle Params.
	const FALSMantleAsset& MantleAsset = GetMantleAsset(MantleType);

	MantleParams.AnimMontage = MantleAsset.AnimMontage;
	MantleParams.PositionCorrectionCurve = MantleAsset.PositionCorrectionCurve;
	MantleParams.StartingOffset = MantleAsset.StartingOffset;
	MantleParams.StartingPosition = FMath::GetMappedRangeValueClamped(FVector2D(MantleAsset.LowHeight, MantleAsset.HighHeight),
		FVector2D(MantleAsset.LowStartPosition, MantleAsset.HighStartPosition), MantleHeight);
	MantleParams.PlayRate = FMath::GetMappedRangeValueClamped(FVector2D(MantleAsset.LowHeight, MantleAsset.HighHeight),
		FVector2D(MantleAsset.LowPlayRate, MantleAsset.HighPlayRate), MantleHeight);

	// Step 2: Convert the world space target to the mantle component's local space for use in moving objects.
	MantleLedgeLS.Component = MantleLedgeWS.Component;
	MantleLedgeLS.Transform = MantleLedgeWS.Transform * MantleLedgeWS.Component->GetComponentToWorld().Inverse();

	// Step 3: Set the Mantle Target and calculate the Starting Offset
	// (offset amount between the actor and target transform).
	MantleTarget = MantleLedgeWS.Transform;
	MantleActualStartOffset = TransfromSub(GetActorTransform(), MantleTarget);

	// Step 4: Calculate the Animated Start Offset from the Target Location.
	// This would be the location the actual animation starts at relative to the Target Transform.
	FVector RotatedVector = MantleTarget.GetRotation().Vector() * MantleParams.StartingOffset.Y;
	RotatedVector.Z = MantleParams.StartingOffset.Z;
	const FTransform StartOffset(MantleTarget.Rotator(), MantleTarget.GetLocation() - RotatedVector, FVector::OneVector);
	MantleAnimatedStartOffset = TransfromSub(StartOffset, MantleTarget);

	// Step 5: Clear the Character Movement Mode and set the Movement State to Mantling
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	SetMovementState(EALSMovementState::Mantling);

	// Step 6: Configure the Mantle Timeline so that it is the same length as the
	// Lerp/Correction curve minus the starting position, and plays at the same speed as the animation.
	// Then start the timeline.
	float MinTime = 0.0f;
	float MaxTime = 0.0f;
	MantleParams.PositionCorrectionCurve->GetTimeRange(MinTime, MaxTime);
	MantleTimeline->SetTimelineLength(MaxTime - MantleParams.StartingPosition);
	MantleTimeline->SetPlayRate(MantleParams.PlayRate);
	MantleTimeline->PlayFromStart();

	// Step 7: Play the Anim Montaget if valid.
	if (IsValid(MantleParams.AnimMontage))
	{
		MainAnimInstance->Montage_Play(
			MantleParams.AnimMontage, MantleParams.PlayRate, EMontagePlayReturnType::MontageLength, MantleParams.StartingPosition, false);
	}

	// Step 8: Prevent Incorrect Rotation
	FRotator ForcedRotation = GetCapsuleComponent()->GetComponentRotation();
	ForcedRotation.Yaw = MantleTarget.GetRotation().Rotator().Yaw;
	GetCapsuleComponent()->SetWorldRotation(ForcedRotation);
}

bool AALSBaseCharacter::MantleCheck(const FALSMantleTraceSettings& TraceSettings, EDrawDebugTrace::Type DebugType)
{
	// Step 1: Trace forward to find a wall / object the character cannot walk on.
	const FVector& CapsuleBaseLocation = GetCapsuleBaseLocation(2.0f, GetCapsuleComponent());
	FVector TraceStart = CapsuleBaseLocation + GetPlayerMovementInput() * -30.0f;
	TraceStart.Z += (TraceSettings.MaxLedgeHeight + TraceSettings.MinLedgeHeight) / 2.0f;
	const FVector TraceEnd = TraceStart + (GetPlayerMovementInput() * TraceSettings.ReachDistance);
	const float HalfHeight = 1.0f + ((TraceSettings.MaxLedgeHeight - TraceSettings.MinLedgeHeight) / 2.0f);

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult HitResult;
	// ECC_GameTraceChannel2 -> Climbable
	World->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity, ECC_GameTraceChannel2,
		FCollisionShape::MakeCapsule(TraceSettings.ForwardTraceRadius, HalfHeight), Params);

	if (!HitResult.IsValidBlockingHit() || GetCharacterMovement()->IsWalkable(HitResult))
	{
		// Not a valid surface to mantle
		return false;
	}

	const FVector InitialTraceImpactPoint = HitResult.ImpactPoint;
	const FVector InitialTraceNormal = HitResult.ImpactNormal;

	// Step 2: Trace downward from the first trace's Impact Point and determine if the hit location is walkable.
	FVector DownwardTraceEnd = InitialTraceImpactPoint;
	DownwardTraceEnd.Z = CapsuleBaseLocation.Z;
	DownwardTraceEnd += InitialTraceNormal * -15.0f;
	FVector DownwardTraceStart = DownwardTraceEnd;
	DownwardTraceStart.Z += TraceSettings.MaxLedgeHeight + TraceSettings.DownwardTraceRadius + 1.0f;

	World->SweepSingleByChannel(HitResult, DownwardTraceStart, DownwardTraceEnd, FQuat::Identity, ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(TraceSettings.DownwardTraceRadius), Params);

	if (!GetCharacterMovement()->IsWalkable(HitResult))
	{
		// Not a valid surface to mantle
		return false;
	}

	const FVector DownTraceLocation(HitResult.Location.X, HitResult.Location.Y, HitResult.ImpactPoint.Z);
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();

	// Step 3: Check if the capsule has room to stand at the downward trace's location.
	// If so, set that location as the Target Transform and calculate the mantle height.
	const FVector& CapsuleLocationFBase = GetCapsuleLocationFromBase(DownTraceLocation, 2.0f, GetCapsuleComponent());
	const bool bCapsuleHasRoom = CapsuleHasRoomCheck(GetCapsuleComponent(), CapsuleLocationFBase, 0.0f, 0.0f, DebugType);

	if (!bCapsuleHasRoom)
	{
		// Capsule doesn't have enough room to mantle
		return false;
	}

	const FTransform TargetTransform(
		(InitialTraceNormal * FVector(-1.0f, -1.0f, 0.0f)).ToOrientationRotator(), CapsuleLocationFBase, FVector::OneVector);

	const float MantleHeight = (CapsuleLocationFBase - GetActorLocation()).Z;

	// Step 4: Determine the Mantle Type by checking the movement mode and Mantle Height.
	EALSMantleType MantleType;
	if (MovementState == EALSMovementState::InAir)
	{
		MantleType = EALSMantleType::FallingCatch;
	}
	else
	{
		MantleType = MantleHeight > 125.0f ? EALSMantleType::HighMantle : EALSMantleType::LowMantle;
	}

	// Step 5: If everything checks out, start the Mantle
	FALSComponentAndTransform MantleWS;
	MantleWS.Component = HitComponent;
	MantleWS.Transform = TargetTransform;
	MantleStart(MantleHeight, MantleWS, MantleType);

	return true;
}

static FTransform MantleComponentLocalToWorld(FALSComponentAndTransform CompAndTransform)
{
	const FTransform& InverseTransform = CompAndTransform.Component->GetComponentToWorld().Inverse();
	const FVector Location = InverseTransform.InverseTransformPosition(CompAndTransform.Transform.GetLocation());
	const FQuat Quat = InverseTransform.InverseTransformRotation(CompAndTransform.Transform.GetRotation());
	const FVector Scale = InverseTransform.InverseTransformPosition(CompAndTransform.Transform.GetScale3D());
	return {Quat, Location, Scale};
}

void AALSBaseCharacter::MantleUpdate(float BlendIn)
{
	// Step 1: Continually update the mantle target from the stored local transform to follow along with moving objects
	MantleTarget = MantleComponentLocalToWorld(MantleLedgeLS);

	// Step 2: Update the Position and Correction Alphas using the Position/Correction curve set for each Mantle.
	const FVector CurveVec =
		MantleParams.PositionCorrectionCurve->GetVectorValue(MantleParams.StartingPosition + MantleTimeline->GetPlaybackPosition());
	const float PositionAlpha = CurveVec.X;
	const float XYCorrectionAlpha = CurveVec.Y;
	const float ZCorrectionAlpha = CurveVec.Z;

	// Step 3: Lerp multiple transforms together for independent control over the horizontal
	// and vertical blend to the animated start position, as well as the target position.

	// Blend into the animated horizontal and rotation offset using the Y value of the Position/Correction Curve.
	const FTransform TargetHzTransform(MantleAnimatedStartOffset.GetRotation(),
		{MantleAnimatedStartOffset.GetLocation().X, MantleAnimatedStartOffset.GetLocation().Y, MantleActualStartOffset.GetLocation().Z},
		FVector::OneVector);
	const FTransform& HzLerpResult = UKismetMathLibrary::TLerp(MantleActualStartOffset, TargetHzTransform, XYCorrectionAlpha);

	// Blend into the animated vertical offset using the Z value of the Position/Correction Curve.
	const FTransform TargetVtTransform(MantleActualStartOffset.GetRotation(),
		{MantleActualStartOffset.GetLocation().X, MantleActualStartOffset.GetLocation().Y, MantleAnimatedStartOffset.GetLocation().Z},
		FVector::OneVector);
	const FTransform& VtLerpResult = UKismetMathLibrary::TLerp(MantleActualStartOffset, TargetVtTransform, ZCorrectionAlpha);

	const FTransform ResultTransform(HzLerpResult.GetRotation(),
		FVector(HzLerpResult.GetLocation().X, HzLerpResult.GetLocation().Y, VtLerpResult.GetLocation().Z), FVector::OneVector);

	// Blend from the currently blending transforms into the final mantle target using the X
	// value of the Position/Correction Curve.
	const FTransform& ResultLerp = UKismetMathLibrary::TLerp(TransfromAdd(MantleTarget, ResultTransform), MantleTarget, PositionAlpha);

	// Initial Blend In (controlled in the timeline curve) to allow the actor to blend into the Position/Correction
	// curve at the midoint. This prevents pops when mantling an object lower than the animated mantle.
	const FTransform& LerpedTarget = UKismetMathLibrary::TLerp(TransfromAdd(MantleTarget, MantleActualStartOffset), ResultLerp, BlendIn);

	// Step 4: Set the actors location and rotation to the Lerped Target.
	SetActorLocationAndTargetRotation(LerpedTarget.GetLocation(), LerpedTarget.GetRotation().Rotator());
}

void AALSBaseCharacter::MantleEnd()
{
	// Set the Character Movement Mode to Walking
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

bool AALSBaseCharacter::CapsuleHasRoomCheck(
	UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset, float RadiusOffset, EDrawDebugTrace::Type DebugType)
{
	// Perform a trace to see if the capsule has room to be at the target location.
	const float ZTarget = Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere() - RadiusOffset + HeightOffset;
	FVector TraceStart = TargetLocation;
	TraceStart.Z += ZTarget;
	FVector TraceEnd = TargetLocation;
	TraceEnd.Z -= ZTarget;
	const float Radius = Capsule->GetUnscaledCapsuleRadius() + RadiusOffset;

	const UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult HitResult;
	World->SweepSingleByProfile(
		HitResult, TraceStart, TraceEnd, FQuat::Identity, FName(TEXT("ALS_Character")), FCollisionShape::MakeSphere(Radius), Params);

	return !(HitResult.bBlockingHit || HitResult.bStartPenetrating);
}

float AALSBaseCharacter::GetMappedSpeed()
{
	// Map the character's current speed to the configured movement speeds with a range of 0-3,
	// with 0 = stopped, 1 = the Walk Speed, 2 = the Run Speed, and 3 = the Sprint Speed.
	// This allows us to vary the movement speeds but still use the mapped range in calculations for consistent results

	const float LocWalkSpeed = CurrentMovementSettings.WalkSpeed;
	const float LocRunSpeed = CurrentMovementSettings.RunSpeed;
	const float LocSprintSpeed = CurrentMovementSettings.SprintSpeed;

	if (Speed > LocRunSpeed)
	{
		return FMath::GetMappedRangeValueClamped(FVector2D(LocRunSpeed, LocSprintSpeed), FVector2D(2.0f, 3.0f), Speed);
	}

	if (Speed > LocWalkSpeed)
	{
		return FMath::GetMappedRangeValueClamped(FVector2D(LocWalkSpeed, LocRunSpeed), FVector2D(1.0f, 2.0f), Speed);
	}

	return FMath::GetMappedRangeValueClamped(FVector2D(0.0f, LocWalkSpeed), FVector2D(0.0f, 1.0f), Speed);
}

EALSGait AALSBaseCharacter::GetAllowedGait()
{
	// Calculate the Allowed Gait. This represents the maximum Gait the character is currently allowed to be in,
	// and can be determined by the desired gait, the rotation mode, the stance, etc. For example,
	// if you wanted to force the character into a walking state while indoors, this could be done here.

	if (Stance == EALSStance::Standing)
	{
		if (RotationMode != EALSRotationMode::Aiming)
		{
			if (DesiredGait == EALSGait::Sprinting)
			{
				return CanSprint() ? EALSGait::Sprinting : EALSGait::Running;
			}
			return DesiredGait;
		}
	}

	// Crouching stance & Aiming rot mode has same behaviour

	if (DesiredGait == EALSGait::Sprinting)
	{
		return EALSGait::Running;
	}

	return DesiredGait;
}

EALSGait AALSBaseCharacter::GetActualGait(EALSGait AllowedGait)
{
	// Get the Actual Gait. This is calculated by the actual movement of the character,  and so it can be different
	// from the desired gait or allowed gait. For instance, if the Allowed Gait becomes walking,
	// the Actual gait will still be running untill the character decelerates to the walking speed.

	const float LocWalkSpeed = CurrentMovementSettings.WalkSpeed;
	const float LocRunSpeed = CurrentMovementSettings.RunSpeed;

	if (Speed > LocRunSpeed + 10.0f)
	{
		if (AllowedGait == EALSGait::Sprinting)
		{
			return EALSGait::Sprinting;
		}
		return EALSGait::Running;
	}

	if (Speed >= LocWalkSpeed + 10.0f)
	{
		return EALSGait::Running;
	}

	return EALSGait::Walking;
}

void AALSBaseCharacter::SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed, float DeltaTime)
{
	// Interpolate the Target Rotation for extra smooth rotation behavior
	TargetRotation = FMath::RInterpConstantTo(TargetRotation, Target, DeltaTime, TargetInterpSpeed);
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, ActorInterpSpeed));
}

float AALSBaseCharacter::CalculateGroundedRotationRate()
{
	// Calculate the rotation rate by using the current Rotation Rate Curve in the Movement Settings.
	// Using the curve in conjunction with the mapped speed gives you a high level of control over the rotation
	// rates for each speed. Increase the speed if the camera is rotating quickly for more responsive rotation.

	const float MappedSpeedVal = GetMappedSpeed();
	const float CurveVal = CurrentMovementSettings.RotationRateCurve->GetFloatValue(MappedSpeedVal);
	const float ClampedAimYawRate = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 300.0f), FVector2D(1.0f, 3.0f), AimYawRate);
	return CurveVal * ClampedAimYawRate;
}

void AALSBaseCharacter::LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed, float DeltaTime)
{
	// Prevent the character from rotating past a certain angle.
	FRotator Delta = GetControlRotation() - GetActorRotation();
	Delta.Normalize();
	const float RangeVal = Delta.Yaw;

	if (RangeVal < AimYawMin || RangeVal > AimYawMax)
	{
		const float ControlRotYaw = GetControlRotation().Yaw;
		const float TargetYaw = ControlRotYaw + (RangeVal > 0.0f ? AimYawMin : AimYawMax);
		SmoothCharacterRotation(FRotator(0.0f, TargetYaw, 0.0f), 0.0f, InterpSpeed, DeltaTime);
	}
}

void AALSBaseCharacter::GetControlForwardRightVector(FVector& Forward, FVector& Right)
{
	const FRotator ControlRot(0.0f, GetControlRotation().Yaw, 0.0f);
	Forward = GetInputAxisValue("MoveForward/Backwards") * UKismetMathLibrary::GetForwardVector(ControlRot);
	Right = GetInputAxisValue("MoveRight/Left") * UKismetMathLibrary::GetRightVector(ControlRot);
}

FVector AALSBaseCharacter::GetPlayerMovementInput()
{
	FVector Forward;
	FVector Right;
	GetControlForwardRightVector(Forward, Right);
	return (Forward + Right).GetSafeNormal();
}

static TPair<float, float> FixDiagonalGamepadValues(const float Y, const float X)
{
	float ResultY = Y * FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 0.6f), FVector2D(1.0f, 1.2f), FMath::Abs(X));
	ResultY = FMath::Clamp(ResultY, -1.0f, 1.0f);
	float ResultX = X * FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 0.6f), FVector2D(1.0f, 1.2f), FMath::Abs(Y));
	ResultX = FMath::Clamp(ResultX, -1.0f, 1.0f);
	return TPair<float, float>(ResultY, ResultX);
}

void AALSBaseCharacter::PlayerForwardMovementInput(float Value)
{
	if (MovementState == EALSMovementState::Grounded || MovementState == EALSMovementState::InAir)
	{
		// // Default camera relative movement behavior
		// const float Scale = FixDiagonalGamepadValues(Value, GetInputAxisValue("MoveRight/Left")).Key;
		// const FRotator DirRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		// AddMovementInput(UKismetMathLibrary::GetForwardVector(DirRotator), Scale);
		AddMovementInput(FVector(0.f, 1.0f, 0.0f), Value);
	}
}

void AALSBaseCharacter::PlayerRightMovementInput(float Value)
{
	if (MovementState == EALSMovementState::Grounded || MovementState == EALSMovementState::InAir)
	{
		// Default camera relative movement behavior
		const float Scale = FixDiagonalGamepadValues(GetInputAxisValue("MoveForward/Backwards"), Value).Value;
		const FRotator DirRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		AddMovementInput(UKismetMathLibrary::GetRightVector(DirRotator), Scale);
	}
}

void AALSBaseCharacter::PlayerCameraUpInput(float Value)
{
	AddControllerPitchInput(LookUpDownRate * Value);
}

void AALSBaseCharacter::PlayerCameraRightInput(float Value)
{
	AddControllerYawInput(LookLeftRightRate * Value);
}

void AALSBaseCharacter::JumpPressedAction()
{
	// Jump Action: Press "Jump Action" to end the ragdoll if ragdolling, check for a mantle if grounded or in air,
	// stand up if crouching, or jump if standing.

	if (MovementAction == EALSMovementAction::None)
	{
		if (MovementState == EALSMovementState::Grounded)
		{
			if (bHasMovementInput)
			{
				if (MantleCheckGrounded())
				{
					return;
				}
			}
			if (Stance == EALSStance::Standing)
			{
				Jump();
			}
			else if (Stance == EALSStance::Crouching)
			{
				UnCrouch();
			}
		}
		else if (MovementState == EALSMovementState::InAir)
		{
			MantleCheckFalling();
		}
		else if (MovementState == EALSMovementState::Ragdoll)
		{
			RagdollEnd();
		}
	}
}

void AALSBaseCharacter::JumpReleasedAction()
{
	StopJumping();
}

void AALSBaseCharacter::SprintPressedAction()
{
	SetDesiredGait(EALSGait::Sprinting);
}

void AALSBaseCharacter::SprintReleasedAction()
{
	SetDesiredGait(EALSGait::Running);
}

void AALSBaseCharacter::AimPressedAction()
{
	// AimAction: Hold "AimAction" to enter the aiming mode, release to revert back the desired rotation mode.
	SetRotationMode(EALSRotationMode::Aiming);
}

void AALSBaseCharacter::AimReleasedAction()
{
	if (ViewMode == EALSViewMode::ThirdPerson)
	{
		SetRotationMode(DesiredRotationMode);
	}
	else if (ViewMode == EALSViewMode::FirstPerson)
	{
		SetRotationMode(EALSRotationMode::LookingDirection);
	}
}

void AALSBaseCharacter::CameraPressedAction()
{
	UWorld* World = GetWorld();
	check(World);
	CameraActionPressedTime = World->GetTimeSeconds();
	GetWorldTimerManager().SetTimer(OnCameraModeSwapTimer, this, &AALSBaseCharacter::OnSwitchCameraMode, ViewModeSwitchHoldTime, false);
}

void AALSBaseCharacter::CameraReleasedAction()
{
	if (ViewMode == EALSViewMode::FirstPerson)
	{
		// Don't swap shoulders on first person mode
		return;
	}

	UWorld* World = GetWorld();
	check(World);
	if (World->GetTimeSeconds() - CameraActionPressedTime < ViewModeSwitchHoldTime)
	{
		// Switch shoulders
		SetRightShoulder(!bRightShoulder);
		GetWorldTimerManager().ClearTimer(OnCameraModeSwapTimer);  // Prevent mode change
	}
}

void AALSBaseCharacter::OnSwitchCameraMode()
{
	// Switch camera mode
	if (ViewMode == EALSViewMode::FirstPerson)
	{
		SetViewMode(EALSViewMode::ThirdPerson);
	}
	else if (ViewMode == EALSViewMode::ThirdPerson)
	{
		SetViewMode(EALSViewMode::FirstPerson);
	}
}

void AALSBaseCharacter::StancePressedAction()
{
	// Stance Action: Press "Stance Action" to toggle Standing / Crouching, double tap to Roll.

	if (MovementAction != EALSMovementAction::None)
	{
		return;
	}

	UWorld* World = GetWorld();
	check(World);

	const float PrevStanceInputTime = LastStanceInputTime;
	LastStanceInputTime = World->GetTimeSeconds();

	if (LastStanceInputTime - PrevStanceInputTime <= RollDoubleTapTimeout)
	{
		// Roll
		OnRoll();

		if (Stance == EALSStance::Standing)
		{
			SetDesiredStance(EALSStance::Crouching);
		}
		else if (Stance == EALSStance::Crouching)
		{
			SetDesiredStance(EALSStance::Standing);
		}
		return;
	}

	if (MovementState == EALSMovementState::Grounded)
	{
		if (Stance == EALSStance::Standing)
		{
			SetDesiredStance(EALSStance::Crouching);
			Crouch();
		}
		else if (Stance == EALSStance::Crouching)
		{
			SetDesiredStance(EALSStance::Standing);
			UnCrouch();
		}
	}

	// Notice: MovementState == EALSMovementState::InAir case is removed
}

void AALSBaseCharacter::WalkPressedAction()
{
	if (DesiredGait == EALSGait::Walking)
	{
		SetDesiredGait(EALSGait::Running);
	}
	else if (DesiredGait == EALSGait::Running)
	{
		SetDesiredGait(EALSGait::Walking);
	}
}

void AALSBaseCharacter::RagdollPressedAction()
{
	// Ragdoll Action: Press "Ragdoll Action" to toggle the ragdoll state on or off.

	if (GetMovementState() == EALSMovementState::Ragdoll)
	{
		RagdollEnd();
	}
	else
	{
		RagdollStart();
	}
}

void AALSBaseCharacter::VelocityDirectionPressedAction()
{
	// Select Rotation Mode: Switch the desired (default) rotation mode to Velocity or Looking Direction.
	// This will be the mode the character reverts back to when un-aiming
	SetDesiredRotationMode(EALSRotationMode::VelocityDirection);
	SetRotationMode(EALSRotationMode::VelocityDirection);
}

void AALSBaseCharacter::LookingDirectionPressedAction()
{
	SetDesiredRotationMode(EALSRotationMode::LookingDirection);
	SetRotationMode(EALSRotationMode::LookingDirection);
}
