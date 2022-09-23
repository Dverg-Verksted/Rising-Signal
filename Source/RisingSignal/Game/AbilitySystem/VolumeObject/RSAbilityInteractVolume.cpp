// It is owned by the company Dverg Verksted.


#include "RSAbilityInteractVolume.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ARSAbilityInteractVolume::ARSAbilityInteractVolume()
{
    PrimaryActorTick.bCanEverTick = false;
     
    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereComponent->InitSphereRadius(SphereRadius);
    SphereComponent->SetCollisionProfileName("CharacterMesh");
    SphereComponent->SetGenerateOverlapEvents(true);
    SphereComponent->bHiddenInGame = false;
    SetRootComponent(SphereComponent);
    
}

void ARSAbilityInteractVolume::BeginPlay()
{
    Super::BeginPlay();

    // Binding event on overlap begin or end
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARSAbilityInteractVolume::OnOverlapBegin);
    SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ARSAbilityInteractVolume::OnOverlapEnd);
   
}

void ARSAbilityInteractVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Call change on overlap begin with params, that set in component
    SetStateChangedValue(OtherActor, AbilityStateType, AddValueToState);
}

void ARSAbilityInteractVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{

    // Call change on overlap begin with params, that set in component, with invert (-)
    SetStateChangedValue(OtherActor, AbilityStateType, -AddValueToState);
}

void ARSAbilityInteractVolume::SetStateChangedValue(const AActor* Actor, const EAbilityStatesType AbilityStType, const float AddValue) const
{
    if(!Actor) return;
    
    URSAbilitySystem* AbilitySystem = Cast<URSAbilitySystem>(Actor->GetComponentByClass(URSAbilitySystem::StaticClass()));
    if(AbilitySystem != nullptr)
    {
        AbilitySystem->SetChangeValue(AbilityStateType, AddValue);
    }
}
