// It is owned by the company Dverg Verksted.

#include "Player/RSGamePLayer.h"
#include "AlsCameraComponent.h"
#include "AlsCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/AbilitySystem/BaseComponents/RSAbilitySystem.h"
#include "GameFramework/GameSession.h"
#include "Library/RSFunctionLibrary.h"
#include "GameFramework/SpringArmComponent.h"

#pragma region Default

ARSGamePLayer::ARSGamePLayer()
{
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

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

    AbilitySystem = CreateDefaultSubobject<URSAbilitySystem>("AbilitySystem");

    GamePlayerController = Cast<ARSGamePlayerController>(GetController());
}

void ARSGamePLayer::BeginPlay()
{
    Super::BeginPlay();

    AbilitySystem->OnStateChangedSignature.AddDynamic(this, &ARSGamePLayer::CheckSomeState);
    AbilitySystem->OnDeath.AddDynamic(this, &ARSGamePLayer::RegisterDeath);
}

void ARSGamePLayer::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ARSGamePLayer::CalcCamera(float DeltaTime, FMinimalViewInfo& ViewInfo)
{
    Super::CalcCamera(DeltaTime, ViewInfo);
}

#pragma endregion

#pragma region PlayerInput

void ARSGamePLayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ThisClass::InputJumpPressed);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ThisClass::InputJumpReleased);

    PlayerInputComponent->BindAction(TEXT("Ragdoll"), IE_Pressed, this, &ThisClass::InputRagdollPressed);

    PlayerInputComponent->BindAction(TEXT("RotationMode"), IE_Pressed, this, &ThisClass::InputRotationModePressed);
    PlayerInputComponent->BindAction(TEXT("ViewMode"), IE_Pressed, this, &ThisClass::InputViewModePressed);
    PlayerInputComponent->BindAction(TEXT("SwitchShoulder"), IE_Pressed, this, &ThisClass::InputSwitchShoulderPressed);

    PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ARSGamePLayer::OpenCloseInventory);
}

void ARSGamePLayer::InputLookUp(const float Value)
{
    AddControllerPitchInput(-Value * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARSGamePLayer::InputLookRight(const float Value)
{
    AddControllerYawInput(Value * LookRightRate * GetWorld()->GetDeltaSeconds());
}

void ARSGamePLayer::InputMoveForward(const float Value)
{
    AddMovementInput(UAlsMath::AngleToDirection2D(GetViewState().Rotation.Yaw),
        UAlsMath::NormalizeInputAxis(Value, GetInputAxisValue("MoveRight")));
}

void ARSGamePLayer::InputMoveRight(const float Value)
{
    AddMovementInput(UAlsMath::AngleToDirection2D(GetViewState().Rotation.Yaw + 90.0f),
        UAlsMath::NormalizeInputAxis(Value, GetInputAxisValue("MoveForward")));
}

void ARSGamePLayer::InputSprintPressed()
{
    // Start the sprint with a slight delay to give the player enough time to start the roll with a double click instead.

    static constexpr auto StartDelay{0.1f};

    GetWorldTimerManager().SetTimer(SprintStartTimer,
        FTimerDelegate::CreateWeakLambda(this, [this]
        {
            SetDesiredGait(EAlsGait::Sprinting);
        }), StartDelay, false);
}

void ARSGamePLayer::InputSprintReleased()
{
    if (GetWorldTimerManager().TimerExists(SprintStartTimer))
    {
        GetWorldTimerManager().ClearTimer(SprintStartTimer);
    }
    else
    {
        SetDesiredGait(EAlsGait::Running);
    }
}

void ARSGamePLayer::InputRoll()
{
    GetWorldTimerManager().ClearTimer(SprintStartTimer);

    static constexpr auto PlayRate{1.3f};

    TryStartRolling(PlayRate);
}

void ARSGamePLayer::InputWalk()
{
    if (GetWorldTimerManager().TimerExists(SprintStartTimer))
    {
        return;
    }

    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    // ReSharper disable once CppIncompleteSwitchStatement
    switch (GetDesiredGait())
    {
        case EAlsGait::Walking: SetDesiredGait(EAlsGait::Running);
            break;

        case EAlsGait::Running: SetDesiredGait(EAlsGait::Walking);
            break;
    }
}

void ARSGamePLayer::InputCrouch()
{
    switch (GetDesiredStance())
    {
        case EAlsStance::Standing: SetDesiredStance(EAlsStance::Crouching);
            break;

        case EAlsStance::Crouching: SetDesiredStance(EAlsStance::Standing);
            break;
    }
}

void ARSGamePLayer::InputJumpPressed()
{
    if (TryStopRagdolling())
    {
        return;
    }

    if (TryStartMantlingGrounded())
    {
        return;
    }

    if (GetStance() == EAlsStance::Crouching)
    {
        SetDesiredStance(EAlsStance::Standing);
        return;
    }

    Jump();
}

void ARSGamePLayer::InputJumpReleased()
{
    StopJumping();
}

void ARSGamePLayer::InputRagdollPressed()
{
    if (!TryStopRagdolling())
    {
        StartRagdolling();
    }
}

void ARSGamePLayer::InputRotationModePressed()
{
    SetDesiredRotationMode(GetDesiredRotationMode() != EAlsRotationMode::VelocityDirection
                               ? EAlsRotationMode::VelocityDirection
                               : EAlsRotationMode::LookingDirection);
}

void ARSGamePLayer::InputViewModePressed()
{
    SetViewMode(GetViewMode() == EAlsViewMode::FirstPerson
                    ? EAlsViewMode::ThirdPerson
                    : EAlsViewMode::FirstPerson);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARSGamePLayer::InputSwitchShoulderPressed()
{
    // AlsCamera->SetRightShoulder(!AlsCamera->IsRightShoulder());
}

void ARSGamePLayer::OpenCloseInventory()
{
}

#pragma endregion

#pragma region Debug

void ARSGamePLayer::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused, float& VerticalPosition)
{
    Super::DisplayDebug(Canvas, DebugDisplay, Unused, VerticalPosition);
}

#pragma endregion

#pragma region Extension

void ARSGamePLayer::CheckSomeState(EStateType StateTyp, float Value)
{
    if (StateTyp == EStateType::Stamina)
    {
        if (Value <= 20)
        {
            GetAlsCharacterMovement()->MaxWalkSpeed = 100.0f;
        }
        else
        {
            GetAlsCharacterMovement()->MaxWalkSpeed = 450.0f;
        }
    }
}

void ARSGamePLayer::RegisterDeath()
{
    // some death logic for player
}

#pragma endregion Extension
