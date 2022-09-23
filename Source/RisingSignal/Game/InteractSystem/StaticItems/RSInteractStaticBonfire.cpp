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

    if (auto InteractActor = Cast<AInteractItemActor>(GetOwner()))
    {
        ParentInteractActor = InteractActor;
    }

    if (bIsFired)
    {
        CheckIfCharactersInsideVolume();

        // ParentInteractActor->SetInteractText(FText::FromString("Сохранить"));
    }
    else
    {
        // ParentInteractActor->SetInteractText(FText::FromString("Зажечь огонь"));
    }
}

void ARSInteractStaticBonfire::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bIsFired) return;

    if (const auto OverlapChar = Cast<ACharacter>(OtherActor))
    {
        CharacterInsideVolume(OverlapChar, false);
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
        AbilitySystem->SetChangeValue(EAbilityStatesType::Temp, HeatTemperIncreaseValue * (bCharInside ? 1 : -1));
    }

    if (const auto CraftComp = Character->FindComponentByClass<URSCraftComponent>())
    {
        CraftComp->SetCampfireNearBy(true);
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
        // ParentInteractActor->SetInteractText(FText::FromString("Сохранить"));
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

    if (PropertyChangedEvent.GetPropertyName() == TEXT("HeatVolumeRadius"))
    {
        HeatVolume->SetSphereRadius(HeatVolumeRadius);
    }

    if (PropertyChangedEvent.Property->GetName() == TEXT("bIsFired"))
    {
        //SetupFireFX(bIsFired)
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
