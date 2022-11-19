// It is owned by the company Dverg Verksted.


#include "Player/NewTestPlayer/RSBaseCharacterAnimInstance.h"

void URSBaseCharacterAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
    CachedBaseCharacter = Cast<ARSBaseCharacter>(TryGetPawnOwner());
    if(CachedBaseCharacter.IsValid())
    {
        CachedBaseCharacter->OnSlide.BindUObject(this, &URSBaseCharacterAnimInstance::PlayRollMontage);
        CachedBaseCharacter->OnLanded.AddUObject(this, &URSBaseCharacterAnimInstance::SetFallHeight);
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
    bIsFalling = CharacterMovementComponent->IsFalling();
}

void URSBaseCharacterAnimInstance::PlayRollMontage()
{
    Montage_Play(RollMontage,1.0f, EMontagePlayReturnType::Duration);
}

void URSBaseCharacterAnimInstance::SetFallHeight(float NewValue)
{
    FallHeight = NewValue;
}
