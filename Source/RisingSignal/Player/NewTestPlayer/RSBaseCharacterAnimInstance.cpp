// It is owned by the company Dverg Verksted.


#include "Player/NewTestPlayer/RSBaseCharacterAnimInstance.h"

void URSBaseCharacterAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
    CachedBaseCharacter = Cast<ARSBaseCharacter>(TryGetPawnOwner());
    if(CachedBaseCharacter.IsValid())
    {
        CachedBaseCharacter->OnLanded.AddUObject(this, &URSBaseCharacterAnimInstance::SetFallHeight);
        CachedBaseCharacter->OnRollStateChangedSignature.BindUObject(this, &URSBaseCharacterAnimInstance::OnRollStateChanged);
    }
}

void URSBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if(!CachedBaseCharacter.IsValid())
    {
        return;
    }

    const URSCharacterMovementComponent* CharacterMovementComponent = CachedBaseCharacter->GetBaseCharacterMovementComponent();

    Speed = FMath::Lerp(Speed, CharacterMovementComponent->Velocity.Size(), LerpAlpha);
    Direction = CalculateDirection(CharacterMovementComponent->Velocity, CachedBaseCharacter->GetActorRotation());
    LadderDirection = CharacterMovementComponent->Velocity.Z;
    bIsFalling = CharacterMovementComponent->IsFalling();
    bIsSprinting = CachedBaseCharacter->GetIsSprinting();
    bIsCrouching = CharacterMovementComponent->IsCrouching();
    bIsOnLadder = CharacterMovementComponent->IsOnLadder();
    HangingSpeed = FMath::Lerp(HangingSpeed, CachedBaseCharacter->GetHangingSpeed(), LerpAlpha);

    LeftFootEffectorLocation = FVector(-(CachedBaseCharacter->GetIKLeftFootOffset() + CachedBaseCharacter->GetIKPelvisOffset()), 0.0f, 0.0f);
    RightFootEffectorLocation = FVector((CachedBaseCharacter->GetIKRightFootOffset() + CachedBaseCharacter->GetIKPelvisOffset()), 0.0f , 0.0f);
    PelvisOffset = FVector(0.0f, 0.0f, CachedBaseCharacter->GetIKPelvisOffset());
}

void URSBaseCharacterAnimInstance::ToggleHanging(bool NewValue)
{
    bIsHanging = NewValue;
}

void URSBaseCharacterAnimInstance::SetFallHeight(float NewValue)
{
    FallHeight = NewValue;
}

void URSBaseCharacterAnimInstance::OnRollStateChanged(bool State)
{
    if(State)
    {
        Montage_Play(RollMontage, 1.0f, EMontagePlayReturnType::Duration);
    }
    else
    {
        Montage_Stop(0.5f, RollMontage);
    }
    
}
