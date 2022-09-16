// It is owned by the company Dverg Verksted.


#include "RSTempVolume.h"
#include "Components/SphereComponent.h"

// Sets default values
ARSTempVolume::ARSTempVolume()
{
    PrimaryActorTick.bCanEverTick = false;
    
    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereComponent->SetSphereRadius(50.0f);
    SphereComponent->UpdateCollisionProfile();
    SetRootComponent(SphereComponent);
    
}

void ARSTempVolume::BeginPlay()
{
    Super::BeginPlay();
    if(SphereComponent->OnComponentBeginOverlap.IsBound())
    {
        SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARSTempVolume::OnOverlapBegin);
    }
    if(SphereComponent->OnComponentEndOverlap.IsBound())
    {
        SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ARSTempVolume::OnOverlapEnd);
    }
}

void ARSTempVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Display, TEXT("Overlap"));
    
}

void ARSTempVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    
}
