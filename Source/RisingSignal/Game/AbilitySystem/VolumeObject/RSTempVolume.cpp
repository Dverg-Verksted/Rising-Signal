// It is owned by the company Dverg Verksted.


#include "RSTempVolume.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ARSTempVolume::ARSTempVolume()
{
    PrimaryActorTick.bCanEverTick = false;
    
        SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
        SphereComponent->InitSphereRadius(SphereRadius);
        SphereComponent->SetCollisionProfileName("CharacterMesh");
        SphereComponent->SetGenerateOverlapEvents(true);
        SphereComponent->bHiddenInGame = false;
        SetRootComponent(SphereComponent);
    
        // BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
        // BoxComponent->InitBoxExtent(FVector(BoxComponentWidth, BoxComponentHeight, BoxComponentDepth));
        // BoxComponent->SetCollisionProfileName("CharacterMesh");
        // BoxComponent->SetGenerateOverlapEvents(true);
        // BoxComponent->bHiddenInGame = false;
        // SetRootComponent(BoxComponent);
    
    
}

void ARSTempVolume::BeginPlay()
{
    Super::BeginPlay();
        SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARSTempVolume::OnOverlapBegin);
        SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ARSTempVolume::OnOverlapEnd);
   
}

void ARSTempVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    SetStateChangedValue(OtherActor, AbilityStateType, ChangedValueModifier);
}

void ARSTempVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    SetStateChangedValue(OtherActor, AbilityStateType, 1 / ChangedValueModifier);
}

void ARSTempVolume::SetStateChangedValue(AActor* Actor, EAbilityStatesType AbilityType, float ChangedValModifier)
{
    URSAbilitySystem* AbilitySystem = Cast<URSAbilitySystem>(Actor->GetComponentByClass(URSAbilitySystem::StaticClass()));
    if(AbilitySystem)
    {
        AbilitySystem->SetChangeValue(AbilityType, ChangedValModifier);
    }
}
