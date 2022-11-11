// It is owned by the company Dverg Verksted.


#include "Player/NewTestPlayer/RSBaseCharacterAnimInstance.h"

void URSBaseCharacterAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
    CachedBaseCharacter = Cast<ARSBaseCharacter>(TryGetPawnOwner());
    if(CachedBaseCharacter.IsValid())
    {
        CachedBaseCharacter->OnSlide.BindUObject(this, &URSBaseCharacterAnimInstance::PlayRollMontage);   
    }
}

void URSBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if(!CachedBaseCharacter.IsValid())
    {
        return;
    }

    
}

void URSBaseCharacterAnimInstance::PlayRollMontage()
{
    Montage_Play(RollMontage,1.0f, EMontagePlayReturnType::Duration);
}
