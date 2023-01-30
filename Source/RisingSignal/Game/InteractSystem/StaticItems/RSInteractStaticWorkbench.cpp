// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/StaticItems/RSInteractStaticWorkbench.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Game/Inventory/RSCraftComponent.h"
#include "Game/InteractSystem/InteractItemActor.h"
#include "GameFramework/Character.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/RSGamePLayer.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"

ARSInteractStaticWorkbench::ARSInteractStaticWorkbench()
{
    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
    RootComponent = BaseMesh;
}

void ARSInteractStaticWorkbench::BeginPlay()
{
    Super::BeginPlay();

    if (const auto InteractActor = Cast<AInteractItemActor>(GetOwner()))
    {
        ParentInteractActor = InteractActor;
        if (const auto VolumeComp = ParentInteractActor->FindComponentByClass<USphereComponent>())
        {
            VolumeComp->OnComponentBeginOverlap.AddDynamic(this, &ARSInteractStaticWorkbench::OnVolumeBeginOverlap);
            VolumeComp->OnComponentEndOverlap.AddDynamic(this, &ARSInteractStaticWorkbench::OnVolumeEndOverlap);
        }
    }
}

void ARSInteractStaticWorkbench::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    const auto OverlapChar = Cast<ACharacter>(OtherActor);

    if (const auto OverlapComp = Cast<UCapsuleComponent>(OtherComp))
    {
        CharacterInsideVolume(OverlapChar, true);
    }
}

void ARSInteractStaticWorkbench::OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    const auto OverlapComp = Cast<UCapsuleComponent>(OtherComp);
    const auto OverlapChar = Cast<ACharacter>(OtherActor);

    if (OverlapComp)
    {
        CharacterInsideVolume(OverlapChar, false);
    }
}

void ARSInteractStaticWorkbench::Interact(ACharacter* InteractingCharacter)
{
    Super::Interact(InteractingCharacter);

    LOG_RS(ELogRSVerb::Warning, InteractingCharacter->GetName());

    if (ARSBaseCharacter* Player = Cast<ARSBaseCharacter>(InteractingCharacter))
    {
        Player->OpenCloseInventory();
    }
}

void ARSInteractStaticWorkbench::CharacterInsideVolume(ACharacter* Character, const bool bCharInside)
{
    if (!Character) return;

    if (const auto CraftComp = Character->FindComponentByClass<URSCraftComponent>())
    {

        CraftComp->SetWorkbenchNearBy(bCharInside);
    }
}

void ARSInteractStaticWorkbench::CheckIfCharactersInsideVolume()
{
    TArray<AActor*> OverlappingActors;
    const auto VolumeComp = ParentInteractActor->FindComponentByClass<USphereComponent>();
    VolumeComp->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

    for (auto OverlapActor : OverlappingActors)
    {
        if (auto OverlapChar = Cast<ACharacter>(OverlapActor))
        {
            CharacterInsideVolume(OverlapChar, true);
        }
    }
}
