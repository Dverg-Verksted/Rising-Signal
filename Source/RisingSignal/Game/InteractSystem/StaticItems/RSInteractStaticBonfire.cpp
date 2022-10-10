// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/StaticItems/RSInteractStaticBonfire.h"

#include "Components/SphereComponent.h"
#include "Game/InteractSystem/InteractItemActor.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/RSGamePLayer.h"

ARSInteractStaticBonfire::ARSInteractStaticBonfire()
{
    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
    RootComponent = BaseMesh;

    HeatVolume = CreateDefaultSubobject<USphereComponent>("HeatVolume");
    HeatVolume->SetupAttachment(BaseMesh);
    HeatVolume->SetSphereRadius(HeatVolumeRadius);
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
        CheckIfCharactersInsideVolume();
    }
}

void ARSInteractStaticBonfire::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bIsFired)
    {
        if (ParentInteractActor)
        {
            ParentInteractActor->SetInteractText(FireOnText);
        }

        if (const auto OverlapChar = Cast<ACharacter>(OtherActor))
        {
            CharacterInsideVolume(OverlapChar, true);
        }
    }
    else
    {
        if (ParentInteractActor)
        {
            ParentInteractActor->SetInteractText(FireOffText);
        }
        return;
    }
}

void ARSInteractStaticBonfire::OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!bIsFired) return;

    if (const auto OverlapChar = Cast<ACharacter>(OtherActor))
    {
        CharacterInsideVolume(OverlapChar, false);
    }
}

void ARSInteractStaticBonfire::CharacterInsideVolume(ACharacter* Character, const bool bCharInside)
{
    if (const auto AbilitySystem = Character->FindComponentByClass<URSAbilitySystem>())
    {
        AbilitySystem->SetChangeValue(EAbilityStatesType::Temp, HeatTemperIncreaseValue * (bCharInside ? -1 : 1));
    }

    if (const auto CraftComp = Character->FindComponentByClass<URSCraftComponent>())
    {
        CraftComp->SetCampfireNearBy(bCharInside);
    }
}

void ARSInteractStaticBonfire::Interact(ACharacter* InteractingCharacter)
{
    Super::Interact(InteractingCharacter);

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
    if (bSetFire == bIsFired) return;

    bIsFired = bSetFire;

    CheckIfCharactersInsideVolume();

    // SetupFireFX()
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

    // LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Name changed property: %s"), *PropertyChangedEvent.Property->GetName()));

    if (PropertyChangedEvent.Property->GetName() == TEXT("HeatVolumeRadius"))
    {
        HeatVolume->SetSphereRadius(HeatVolumeRadius);
    }

    if (PropertyChangedEvent.Property->GetName() == TEXT("bIsFired"))
    {
        //SetupFireFX(bIsFired)

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
