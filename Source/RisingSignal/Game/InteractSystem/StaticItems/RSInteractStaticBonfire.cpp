// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/StaticItems/RSInteractStaticBonfire.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Game/InteractSystem/InteractItemActor.h"
#include "Library/RSFunctionLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/RSGamePLayer.h"

ARSInteractStaticBonfire::ARSInteractStaticBonfire()
{
    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
    RootComponent = BaseMesh;

    HeatVolume = CreateDefaultSubobject<USphereComponent>("HeatVolume");
    HeatVolume->SetupAttachment(BaseMesh);
    HeatVolume->SetSphereRadius(HeatVolumeRadius);

    FireVFX = CreateDefaultSubobject<UParticleSystemComponent>("Fire");
    FireVFX->SetupAttachment(RootComponent);

    SmokeVFX = CreateDefaultSubobject<UParticleSystemComponent>("Smoke");
    SmokeVFX->SetupAttachment(FireVFX);
}

void ARSInteractStaticBonfire::BeginPlay()
{
    Super::BeginPlay();

    HeatVolume->OnComponentBeginOverlap.AddDynamic(this, &ARSInteractStaticBonfire::OnVolumeBeginOverlap);
    HeatVolume->OnComponentEndOverlap.AddDynamic(this, &ARSInteractStaticBonfire::OnVolumeEndOverlap);

    if (const auto InteractActor = Cast<AInteractItemActor>(GetOwner()))
    {
        ParentInteractActor = InteractActor;
    }

    if (bIsFired)
    {
        FTimerHandle TempTimerHandle;
        GetWorldTimerManager().SetTimer(TempTimerHandle, this, &ARSInteractStaticBonfire::CheckIfCharactersInsideVolume, 0.1);
    }

    SetEnabledVFX(bIsFired);
}

void ARSInteractStaticBonfire::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bIsFired)
    {
        const auto OverlapComp = Cast<UCapsuleComponent>(OtherComp);
        const auto OverlapChar = Cast<ACharacter>(OtherActor);
        if (OverlapComp)
        {
            if (ParentInteractActor)
            {
                ParentInteractActor->SetInteractText(FireOnText);
            }

            CharacterInsideVolume(OverlapChar, true);
        }
    }
    else
    {
        if (ParentInteractActor)
        {
            ParentInteractActor->SetInteractText(FireOffText);
        }
    }
}

void ARSInteractStaticBonfire::OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!bIsFired) return;

    const auto OverlapComp = Cast<UCapsuleComponent>(OtherComp);
    const auto OverlapChar = Cast<ACharacter>(OtherActor);

    if (OverlapComp && OverlapChar)
    {
        CharacterInsideVolume(OverlapChar, false);
    }
}

void ARSInteractStaticBonfire::CharacterInsideVolume(ACharacter* Character, const bool bCharInside)
{
    if (const auto AbilitySystem = Character->FindComponentByClass<URSAbilitySystem>())
    {
        AbilitySystem->SetChangeValue(EAbilityStatesType::Temp, HeatTemperIncreaseValue * (bCharInside ? 1 : -1));
    }

    if (const auto CraftComp = Character->FindComponentByClass<URSCraftComponent>())
    {
        switch (BonfireType)
        {
            case EBonfireType::SmallFire:
            {
                CraftComp->SetSmallFireNearBy(bCharInside);
                break;
            }
            case EBonfireType::BigFire:
            {
                CraftComp->SetCampfireNearBy(bCharInside);

                break;
            }
            default:
            {
            }
        }
    }
}

void ARSInteractStaticBonfire::SetEnabledVFX(bool bEnable)
{
    if (bEnable)
    {
        FireVFX->ActivateSystem();
        SmokeVFX->ActivateSystem();
    }
    else
    {
        FireVFX->DeactivateSystem();
        SmokeVFX->DeactivateSystem();
    }
}

void ARSInteractStaticBonfire::Interact(ACharacter* InteractingCharacter)
{
    Super::Interact(InteractingCharacter);

    if (bNeedItem) return;

    if (bIsFired)
    {
        // Save();
    }
    else
    {
        SetFire(true);
        if (ParentInteractActor)
        {
            ParentInteractActor->SetInteractText(FireOnText);
            ParentInteractActor->LoadInteractWidget();
        }
    }
}

void ARSInteractStaticBonfire::SetFire(bool bSetFire)
{
    SetEnabledVFX(bSetFire);

    if (bSetFire == bIsFired) return;

    bIsFired = bSetFire;

    CheckIfCharactersInsideVolume();
}

#if UE_EDITOR

void ARSInteractStaticBonfire::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (!PropertyChangedEvent.Property)
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Interact actor: %s have property is nullptr"), *GetName()));
        return;
    }

    if (PropertyChangedEvent.Property->GetName() == TEXT("HeatVolumeRadius"))
    {
        HeatVolume->SetSphereRadius(HeatVolumeRadius);
    }

    if (PropertyChangedEvent.Property->GetName() == TEXT("bIsFired"))
    {
        SetEnabledVFX(bIsFired);

        if (ParentInteractActor)
        {
            ParentInteractActor->SetInteractText(bIsFired ? FireOnText : FireOffText);
        }
    }
}

#endif

void ARSInteractStaticBonfire::CheckIfCharactersInsideVolume()
{
    if (!bIsFired) return;

    TArray<AActor*> OverlappingActors;
    HeatVolume->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

    for (auto OverlapActor : OverlappingActors)
    {
        if (auto OverlapChar = Cast<ACharacter>(OverlapActor))
        {
            CharacterInsideVolume(OverlapChar, true);
        }
    }
}
