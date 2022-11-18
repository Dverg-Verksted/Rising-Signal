// It is owned by the company Dverg Verksted.


#include "Player/NewTestPlayer/RSBaseCharacter.h"
#include "RSLedgeDetectorComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "Game/Inventory/RSCraftComponent.h"
#include "Game/Inventory/RSEquipmentComponent.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "Game/WeaponSystem/WeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
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

    const ACharacter* DefaultChar = GetDefault<ACharacter>(GetClass());
    if(GetMesh() && DefaultChar->GetMesh())
    {
        FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
        MeshRelativeLocation.Z = DefaultChar->GetMesh()->GetRelativeLocation().Z + HalfHeightAdjust;
        BaseTranslationOffset.Z = MeshRelativeLocation.Z;
    }
    else
    {
        BaseTranslationOffset.Z = DefaultChar->GetBaseTranslationOffset().Z + HalfHeightAdjust;
    }
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

}

void ARSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ThisClass::InputLookUp);
    PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ThisClass::InputLookRight);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ThisClass::InputMoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ThisClass::InputMoveRight);

    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ThisClass::InputSprintPressed);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ThisClass::InputSprintReleased);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_DoubleClick, this, &ThisClass::InputRoll);

    PlayerInputComponent->BindAction(TEXT("Walk"), IE_Pressed, this, &ThisClass::InputWalk);
    PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ThisClass::InputCrouch);

    PlayerInputComponent->BindAction(TEXT("Mantle"), IE_Pressed, this, &ThisClass::InputMantle);

    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ThisClass::InputJumpPressed);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ThisClass::InputJumpReleased);

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
    FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
    FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
    AddMovementInput(ForwardVector, Value);
}

void ARSBaseCharacter::InputMoveRight(float Value)
{
    FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
    FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
    AddMovementInput(RightVector, Value);
}

void ARSBaseCharacter::InputSprintPressed()
{
}

void ARSBaseCharacter::InputSprintReleased()
{
}

void ARSBaseCharacter::InputRoll()
{
    RSCharacterMovementComponent->StartRoll();
}

void ARSBaseCharacter::InputWalk()
{
}

void ARSBaseCharacter::InputCrouch()
{
}

void ARSBaseCharacter::InputMantle()
{
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

