// It is owned by the company Dverg Verksted.


#include "Player/NewTestPlayer/RSBaseCharacter.h"
#include "RSBaseCharacterAnimInstance.h"
#include "RSLedgeDetectorComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "Game/InteractSystem/Environment/Ladder/Ladder.h"
#include "Game/InteractSystem/Environment/Rope/Rope.h"
#include "Game/Inventory/RSCraftComponent.h"
#include "Game/Inventory/RSEquipmentComponent.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "Game/WeaponSystem/WeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/RSGamePlayerController.h"

ARSBaseCharacter::ARSBaseCharacter(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer.SetDefaultSubobjectClass<URSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
    
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetMesh());
    SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
    SpringArm->SocketOffset = FVector(0.0f, 0.0f, 50.0f);
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 10.0f;
    SpringArm->CameraLagMaxDistance = 100.0f;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritRoll = false;
    SpringArm->bInheritYaw = false;
    SpringArm->TargetArmLength = 700.0f;
    SpringArm->bDoCollisionTest = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

    AbilitySystem = CreateDefaultSubobject<URSAbilitySystem>("AbilitySystem");

    GamePlayerController = Cast<ARSGamePlayerController>(GetController());

    RSCharacterMovementComponent = StaticCast<URSCharacterMovementComponent*>(GetCharacterMovement());
    InventoryComponent = CreateDefaultSubobject<URSInventoryComponent>("InventoryComponent");
    EquipmentComponent = CreateDefaultSubobject<URSEquipmentComponent>("EquipmentComponent");
    CraftComponent = CreateDefaultSubobject<URSCraftComponent>("CraftComponent");
    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
    LedgeDetectorComponent = CreateDefaultSubobject<URSLedgeDetectorComponent>("LedgeDetectorComponent");

    ExtraSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ExtraMesh");
    ExtraSkeletalMesh->SetupAttachment(GetMesh());
}

void ARSBaseCharacter::Falling()
{
    Super::Falling();
    GetCharacterMovement()->bNotifyApex = true;
}

void ARSBaseCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    const float FallHeight = (CurrentHeight - GetActorLocation().Z) * 0.01;
    if(OnLanded.IsBound())
    {
        OnLanded.Broadcast(FallHeight);
    }
    if(FallHeight > MinHeightForRoll && FallHeight < MaxHeightForRoll)
    {
        Roll();
    }
    if(IsValid(FallDamageCurve))
    {
        const float DamageAmount = FallDamageCurve->GetFloatValue(FallHeight);
        AbilitySystem->ChangeCurrentStateValue(EAbilityStatesType::Health, -DamageAmount);
    }
}

void ARSBaseCharacter::NotifyJumpApex()
{
    Super::NotifyJumpApex();
    CurrentHeight = GetActorLocation().Z;
}

void ARSBaseCharacter::Jump()
{
    Super::Jump();

    if(bIsHanging)
    {
        GetCapsuleComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        RSCharacterMovementComponent->SetMovementMode(MOVE_Walking);
        FRotator CurrentCharacterRotation = GetActorRotation();
        SetActorRotation(FRotator(0.0f, CurrentCharacterRotation.Yaw, 0.0f));
        bIsHanging = false;
        URSBaseCharacterAnimInstance* AnimInstance = Cast<URSBaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
        AnimInstance->ToggleHanging(false);
        ARope* CurrentRope = GetAvailableRope();
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetWorld()->GetTimerManager().SetTimer(HangTimer, [=]()
        {
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        }, 0.5f, false);
        FVector RopePhysicLinearVelocity = CurrentRope->GetCableEndMeshComponent()->GetPhysicsLinearVelocity();
        LaunchCharacter(FVector(RopePhysicLinearVelocity.X, RopePhysicLinearVelocity.Y * CurrentRope->GetLaunchY(), CurrentRope->GetLaunchZ()), false, false);
    }

    if(RSCharacterMovementComponent->IsOnLadder())
    {
        if(GetBaseCharacterMovementComponent()->IsOnLadder())
        {
            GetBaseCharacterMovementComponent()->DetachFromLadder(EDetachFromLadderMethod::JumpOff);
        }
    }

    if(bIsClimbingOnWall)
    {
        RSCharacterMovementComponent->SetMovementMode(MOVE_Walking);
        RSCharacterMovementComponent->bOrientRotationToMovement = true;
        bIsClimbingOnWall = false;
        URSBaseCharacterAnimInstance* AnimInstance = Cast<URSBaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
        AnimInstance->ToggleOnWall(false);
    }
}

void ARSBaseCharacter::Mantle(bool bForce)
{
    if(!(CanMantle() && RSCharacterMovementComponent->IsFalling() || bForce))
    {
        return;
    }
    
    FLedgeDescription LedgeDescription;
	if(LedgeDetectorComponent->LedgeDetect(LedgeDescription))
	{
	    ACharacter* DefaultCharacter = GetClass()->GetDefaultObject<ACharacter>();
	    float DefaultCapsuleHalfHeight = DefaultCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	    float CurrentCapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	    float CapsuleHalfHeightOffset = 0.0f;
	    if(bIsCrouched)
	    {
	        UnCrouch();
	        CapsuleHalfHeightOffset = DefaultCapsuleHalfHeight - CurrentCapsuleHalfHeight;
	    }

	    bIsMantling = true;
		
	    FMantlingMovementParameters MantlingParameters;
	    MantlingParameters.InitialLocation = GetActorLocation() + CapsuleHalfHeightOffset;
	    MantlingParameters.InitialRotation = GetActorRotation();
	    MantlingParameters.TargetLocation = LedgeDescription.Location;
	    MantlingParameters.TargetRotation = LedgeDescription.Rotation;
	    MantlingParameters.PrimitiveComponent = LedgeDescription.PrimitiveComponent;
	    MantlingParameters.PrimitiveComponentInitialLocation = LedgeDescription.PrimitiveInitialLocation;
	    float MantlingHeight = ((MantlingParameters.TargetLocation - DefaultCapsuleHalfHeight * FVector::UpVector) - (MantlingParameters.InitialLocation - DefaultCapsuleHalfHeight * FVector::UpVector)).Z;
	    const FMantlingSettings& MantlingSettings = GetMantlingSettings(MantlingHeight);
	    float MinRange;
	    float MaxRange;
	    MantlingSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange);
	    MantlingParameters.Duration = MaxRange - MinRange;
		
	    MantlingParameters.MantlingCurve = MantlingSettings.MantlingCurve;
	    FVector2D SourceRange(MantlingSettings.MinHeight, MantlingSettings.MaxHeight);
	    FVector2D TargetRange(MantlingSettings.MinHeightStartTime, MantlingSettings.MaxHeightStartTime);
	    MantlingParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, MantlingHeight);

	    MantlingParameters.InitialAnimationLocation = MantlingParameters.TargetLocation - MantlingSettings.AnimationCorrectionZ * FVector::UpVector + MantlingSettings.AnimationCorrectionXY * LedgeDescription.LedgeNormal;
	    
	    RSCharacterMovementComponent->StartMantle(MantlingParameters);
		
		
	    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	    AnimInstance->Montage_Play(MantlingSettings.MantlingMontage, 1.0f, EMontagePlayReturnType::Duration, MantlingParameters.StartTime);
	}
}

void ARSBaseCharacter::Roll()
{
    if(!CanRoll())
    {
        return;
    }
    if(OnRollStateChangedSignature.IsBound())
    {
        OnRollStateChangedSignature.Execute(true);
    }
    RSCharacterMovementComponent->StartRoll();
}

bool ARSBaseCharacter::GetIsMantling() const
{
    return bIsMantling;
}

void ARSBaseCharacter::SetIsMantling(bool NewValue)
{
    bIsMantling = NewValue;
}

bool ARSBaseCharacter::GetIsRolling() const
{
    return bIsRolling;
}

void ARSBaseCharacter::SetIsRolling(bool NewValue)
{
    bIsRolling = NewValue;
}

bool ARSBaseCharacter::GetIsSprinting() const
{
    return bIsSprinting;
}

void ARSBaseCharacter::OnStartRoll(float HalfHeightAdjust)
{
    RecalculateBaseEyeHeight();

    if(OnSlide.IsBound())
    {
        OnSlide.Execute();
    }

    const ACharacter* DefaultChar = GetDefault<ACharacter>(GetClass());
    if(GetMesh() && DefaultChar->GetMesh())
    {
        FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
        MeshRelativeLocation.Z = DefaultChar->GetMesh()->GetRelativeLocation().Z + HalfHeightAdjust + RSCharacterMovementComponent->GetRollCapsuleHalfHeight();
        BaseTranslationOffset.Z = MeshRelativeLocation.Z;
    }
    else
    {
        BaseTranslationOffset.Z = DefaultChar->GetBaseTranslationOffset().Z + HalfHeightAdjust + RSCharacterMovementComponent->GetRollCapsuleHalfHeight();
    }
}

void ARSBaseCharacter::OnStopRoll(float HalfHeightAdjust)
{
    RecalculateBaseEyeHeight();

    if(OnRollStateChangedSignature.IsBound())
    {
        OnRollStateChangedSignature.Execute(false);
    }

    const ACharacter* DefaultChar = GetDefault<ACharacter>(GetClass());
    if(GetMesh() && DefaultChar->GetMesh())
    {
        FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
        MeshRelativeLocation.Z = DefaultChar->GetMesh()->GetRelativeLocation().Z + HalfHeightAdjust;
        BaseTranslationOffset.Z = MeshRelativeLocation.Z;
    }
}

void ARSBaseCharacter::ClimbLadder(float Value)
{
    if(!FMath::IsNearlyZero(Value) && GetBaseCharacterMovementComponent()->IsOnLadder())
    {
        FVector LadderUpVector = GetBaseCharacterMovementComponent()->GetCurrentLadder()->GetActorUpVector();
        AddMovementInput(LadderUpVector, Value);
    }
}

void ARSBaseCharacter::SwingRope(float Value)
{
    if(!FMath::IsNearlyZero(Value) && bIsHanging)
    {
        ARope* CurrentRope = GetAvailableRope();
        FVector RightVector;
        GetControlRightVector(RightVector);
        RightVector *= Value;
        HangingSpeed = Value * 600.0f;
        if(IsValid(CurrentRope))
        {
            CurrentRope->AddSwingForce(RightVector, false);
        }
    }
}

void ARSBaseCharacter::MoveWallForward(float Value)
{
    if(!FMath::IsNearlyZero(Value))
    {
        FVector WallUpVector = GetBaseCharacterMovementComponent()->GetCurrentWall()->GetActorUpVector();
        AddMovementInput(WallUpVector, Value);
    }
}

void ARSBaseCharacter::MoveWallRight(float Value)
{
    if(!FMath::IsNearlyZero(Value))
    {
        FVector WallRightVector = GetBaseCharacterMovementComponent()->GetCurrentWall()->GetActorRightVector();
        AddMovementInput(-WallRightVector, Value);
    }
}

void ARSBaseCharacter::RegisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
    AvailableInteractiveActors.AddUnique(InteractiveActor);
}

void ARSBaseCharacter::UnRegisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
    AvailableInteractiveActors.RemoveSingleSwap(InteractiveActor);
}

void ARSBaseCharacter::InteractWithLadder()
{
    const ALadder* AvailableLadder = GetAvailableLadder();
    if(IsValid(AvailableLadder))
    {
        GetBaseCharacterMovementComponent()->AttachToLadder(AvailableLadder);
    }
}

void ARSBaseCharacter::InteractWithRope(ARope* Rope)
{
    if(!bIsHanging)
    {
        RSCharacterMovementComponent->StopMovementImmediately();
        FRotator RopeRotation = Rope->GetCableEndMeshComponent()->GetComponentRotation();
        SetActorRotation(FRotator(RopeRotation.Pitch, GetActorRotation().Yaw, GetActorRotation().Roll));
        GetCapsuleComponent()->AttachToComponent(Rope->GetCableEndMeshComponent(), FAttachmentTransformRules::KeepWorldTransform);
        URSBaseCharacterAnimInstance* AnimInstance = Cast<URSBaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
        AnimInstance->ToggleHanging(true);
        bIsHanging = true;
        RSCharacterMovementComponent->SetMovementMode(MOVE_Custom, StaticCast<uint8>(ECustomMovementMode::CMOVE_OnRope));
    }
}

void ARSBaseCharacter::InteractWithWall()
{
    if(!bIsClimbingOnWall)
    {
        const AClimbingWall* AvailableWall = GetAvailableWall();
        if(IsValid(AvailableWall))
        {
            bIsClimbingOnWall = true;
            URSBaseCharacterAnimInstance* AnimInstance = Cast<URSBaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
            AnimInstance->ToggleOnWall(true);
            RSCharacterMovementComponent->AttachToWall(AvailableWall);
        }
    }
}

void ARSBaseCharacter::DetachFromWall()
{
    RSCharacterMovementComponent->bOrientRotationToMovement = true;
    bIsClimbingOnWall = false;
    URSBaseCharacterAnimInstance* AnimInstance = Cast<URSBaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
    AnimInstance->ToggleOnWall(false);
    FRotator CurrentCharacterRotation = GetActorRotation();
    SetActorRotation(FRotator(0.0f, CurrentCharacterRotation.Yaw, 0.0f));
}

void ARSBaseCharacter::UpdateCameraRotation()
{
    Camera->SetWorldRotation(FRotator{0,0,0});
}

void ARSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    AbilitySystem->OnStateChangedSignature.AddDynamic(this, &ARSBaseCharacter::CheckSomeState);
    AbilitySystem->OnDeath.AddDynamic(this, &ARSBaseCharacter::RegisterDeath);
}

void ARSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    Mantle();
    CalculateOffsets(DeltaTime);
}

void ARSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ThisClass::InputLookUp);
    PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ThisClass::InputLookRight);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ThisClass::InputMoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ThisClass::InputMoveRight);

    PlayerInputComponent->BindAxis(TEXT("ClimbLadder"), this, &ThisClass::InputLadder);
    PlayerInputComponent->BindAxis(TEXT("SwingLadder"), this, &ThisClass::InputRope);

    PlayerInputComponent->BindAxis(TEXT("MoveWallForward"), this, &ThisClass::InputWallForward);
    PlayerInputComponent->BindAxis(TEXT("MoveWallRight"), this, &ThisClass::InputWallRight);

    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ThisClass::InputSprintPressed);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ThisClass::InputSprintReleased);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_DoubleClick, this, &ThisClass::InputRoll);

    PlayerInputComponent->BindAction(TEXT("Walk"), IE_Pressed, this, &ThisClass::InputWalk);
    PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ThisClass::InputCrouch);

    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ARSBaseCharacter::InputSprintPressed);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ARSBaseCharacter::InputSprintReleased);

    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ThisClass::InputJumpPressed);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ThisClass::InputJumpReleased);

    PlayerInputComponent->BindAction(TEXT("InteractLadder"), IE_Released, this, &ThisClass::InteractWithLadder);

    PlayerInputComponent->BindAction(TEXT("Ragdoll"), IE_Pressed, this, &ThisClass::InputRagdollPressed);
    PlayerInputComponent->BindAction(TEXT("Roll"), IE_Pressed, this, &ARSBaseCharacter::InputRoll);

    PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ThisClass::OpenCloseInventory);

    PlayerInputComponent->BindAction(TEXT("Journal"), IE_Pressed, this, &ThisClass::OpenCloseJournal);

    PlayerInputComponent->BindAction(TEXT("ActionSlot1"), IE_Pressed, this, &ThisClass::InputActionSlot1);
    PlayerInputComponent->BindAction(TEXT("ActionSlot2"), IE_Pressed, this, &ThisClass::InputActionSlot2);
    PlayerInputComponent->BindAction(TEXT("ActionSlot3"), IE_Pressed, this, &ThisClass::InputActionSlot3);
    PlayerInputComponent->BindAction(TEXT("ActionSlot4"), IE_Pressed, this, &ThisClass::InputActionSlot4);
}

void ARSBaseCharacter::InputLookUp(float Value)
{
    AddControllerPitchInput(-Value * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARSBaseCharacter::InputLookRight(float Value)
{
    AddControllerYawInput(Value * LookRightRate * GetWorld()->GetDeltaSeconds());
}

void ARSBaseCharacter::InputMoveForward(float Value)
{
    if(CanMove())
    {
        FRotator YawRotator(0.0f, 0.0f, 0.0f);
        FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
        AddMovementInput(ForwardVector, Value);
    }
}

void ARSBaseCharacter::InputMoveRight(float Value)
{
    if(CanMove())
    {
        FRotator YawRotator(0.0f, 0.0f, 0.0f);
        FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
        AddMovementInput(RightVector, Value);
    }
}

void ARSBaseCharacter::InputLadder(float Value)
{
    ClimbLadder(Value);
}

void ARSBaseCharacter::InputRope(float Value)
{
    SwingRope(Value);
}

void ARSBaseCharacter::InputWallForward(float Value)
{
    if(bIsClimbingOnWall)
    MoveWallForward(Value);
}

void ARSBaseCharacter::InputWallRight(float Value)
{
    if(bIsClimbingOnWall)
    MoveWallRight(Value);
}

void ARSBaseCharacter::InputSprintPressed()
{
    bIsSprinting = true;
}

void ARSBaseCharacter::InputSprintReleased()
{
    bIsSprinting = false;
}

void ARSBaseCharacter::InputRoll()
{
    Roll();
}

void ARSBaseCharacter::InputWalk()
{
}

void ARSBaseCharacter::InputCrouch()
{
    if(GetBaseCharacterMovementComponent()->IsCrouching())
    {
        UnCrouch();
    }
    else
    {
        Crouch();
    }
}

void ARSBaseCharacter::InputInteractLadder()
{
    InteractWithLadder();
}

void ARSBaseCharacter::InputJumpPressed()
{
    Jump();
}

void ARSBaseCharacter::InputJumpReleased()
{
}

void ARSBaseCharacter::InputRagdollPressed()
{
}

void ARSBaseCharacter::InputActionSlot1()
{
    EquipmentComponent->TakeInHands(EquipSlot1);
}

void ARSBaseCharacter::InputActionSlot2()
{
    EquipmentComponent->TakeInHands(EquipSlot2);
}

void ARSBaseCharacter::InputActionSlot3()
{
    EquipmentComponent->TakeInHands(EquipSlot3);
}

void ARSBaseCharacter::InputActionSlot4()
{
    EquipmentComponent->TakeInHands(EquipSlot4);
}

float ARSBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ARSBaseCharacter::OpenCloseInventory()
{
    if (!InventoryOpenClose.IsBound()) return;
    InventoryOpenClose.Broadcast();
}

void ARSBaseCharacter::OpenCloseJournal()
{
    if (!JournalOpenClose.IsBound()) return;
        JournalOpenClose.Broadcast();
}

void ARSBaseCharacter::CheckSomeState(EAbilityStatesType StateTyp, float Value)
{
    if (StateTyp == EAbilityStatesType::Stamina)
    {
        if (Value <= 20)
        {
            canRun = false;
            InputSprintReleased();
        }
        else
        {
            canRun = true;
        }
    }
}

void ARSBaseCharacter::RegisterDeath()
{
    // some death logic for player
    this->GetCharacterMovement()->DisableMovement();
    this->SetLifeSpan(5);
    this->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    this->GetMesh()->SetSimulatePhysics(true);
    this->DisableInput(this->GetController<APlayerController>());
}

const FMantlingSettings& ARSBaseCharacter::GetMantlingSettings(float LedgeHeight) const
{
	    return LedgeHeight > LowMantleMaxHeight ? HighMantlingSettings : LowMantlingSettings;
}

void ARSBaseCharacter::GetControlRightVector(FVector& Right)
{
    Right = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
}

const ALadder* ARSBaseCharacter::GetAvailableLadder() const
{
    const ALadder* Result = nullptr;
    for(const AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
    {
        if(InteractiveActor->IsA<ALadder>())
        {
            Result = StaticCast<const ALadder*>(InteractiveActor);
            break;
        }
    }
    return Result;
}

ARope* ARSBaseCharacter::GetAvailableRope() const
{
    ARope* Result = nullptr;
    for(AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
    {
        if(InteractiveActor->IsA<ARope>())
        {
            Result = StaticCast<ARope*>(InteractiveActor);
            break;
        }
    }
    return Result;
}

AClimbingWall* ARSBaseCharacter::GetAvailableWall() const
{
    AClimbingWall* Result = nullptr;
    for(AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
    {
        if(InteractiveActor->IsA<AClimbingWall>())
        {
            Result = StaticCast<AClimbingWall*>(InteractiveActor);
            break;
        }
    }
    return Result;
}

bool ARSBaseCharacter::CanMove()
{
    if(bIsMantling)
    {
        return false;
    }
    if(bIsRolling)
    {
        return false;
    }
    if(RSCharacterMovementComponent->IsOnLadder())
    {
        return false;
    }
    if(bIsHanging)
    {
        return false;
    }
    if(bIsClimbingOnWall)
    {
        return false;
    }

    return true;
}


bool ARSBaseCharacter::CanMantle()
{
    if(bIsMantling)
    {
        return false;
    }
    if(RSCharacterMovementComponent->IsOnLadder())
    {
        return false;
    }

    return true;
}

bool ARSBaseCharacter::CanRoll()
{
    if(bIsRolling)
    {
        return false;
    }
    if(bIsMantling)
    {
        return false;
    }
    if(RSCharacterMovementComponent->IsOnLadder())
    {
        return false;
    }

    return true;
}

float ARSBaseCharacter::GetIKFootOffset(const FName& SocketName)
{
    float Result = 0.0f;

    CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
    FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);
    FVector LineStartPosition(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z);
    FVector LineEndPostion = LineStartPosition - (CapsuleRadius + IKTraceDistance) * FVector::UpVector;

    FHitResult HitResult;
    ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);

    FVector BoxFoot(2.0f, 15.0f, 7.0f);
    if(UKismetSystemLibrary::BoxTraceSingle(GetWorld(), LineStartPosition, LineEndPostion, BoxFoot, GetMesh()->GetSocketRotation(SocketName), TraceType, true, TArray<AActor*>(), EDrawDebugTrace::None, HitResult, true))
    {
        Result = LineStartPosition.Z - CapsuleRadius - HitResult.Location.Z;
    }
    return Result;
}

float ARSBaseCharacter::GetPelvisOffset()
{
    if(IKLeftFootOffset > MinimumOffset || IKRightFootOffset > MinimumOffset)
    {
        return -FMath::Max(IKLeftFootOffset ,IKRightFootOffset);
    }
    
    return 0.0f;
    
}

void ARSBaseCharacter::CalculateOffsets(float DeltaSeconds)
{
    IKLeftFootOffset = FMath::FInterpTo(IKLeftFootOffset, GetIKFootOffset(IKLeftFootSocketName), DeltaSeconds, IKInterpSpeed);
    IKRightFootOffset = FMath::FInterpTo(IKRightFootOffset, GetIKFootOffset(IKRightFootSocketName), DeltaSeconds, IKInterpSpeed);
    IKPelvisOffset = FMath::FInterpTo(IKPelvisOffset, GetPelvisOffset(), DeltaSeconds, IKInterpSpeed);
}

