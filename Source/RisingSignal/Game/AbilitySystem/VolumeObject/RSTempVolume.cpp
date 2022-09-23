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
    
}

void ARSTempVolume::BeginPlay()
{
    Super::BeginPlay();

    // Привязка событий на пересечение коллизии компонента
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARSTempVolume::OnOverlapBegin);
    SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ARSTempVolume::OnOverlapEnd);
   
}

void ARSTempVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Вызываем изменение при входе с параметром, указанным в компоненте 
    SetStateChangedValue(OtherActor, AbilityStateType, AddValueToState);
}

void ARSTempVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    // Вызываем изменение при выходе с тем же парамтером, но со знаком минус,
    // для возврата к тому значению, которое было до входа в волюм
    SetStateChangedValue(OtherActor, AbilityStateType, -AddValueToState);
}

void ARSTempVolume::SetStateChangedValue(const AActor* Actor, const EAbilityStatesType AbilityStType, const float AddValue) const
{
    // Если возможно получить абилити систем в актере, то вызываем изменение параметра системы по
    // входным в метод параметрам 
    URSAbilitySystem* AbilitySystem = Cast<URSAbilitySystem>(Actor->GetComponentByClass(URSAbilitySystem::StaticClass()));
    if(AbilitySystem)
    {
        AbilitySystem->SetChangeValue(AbilityStateType, AddValue);
    }
}
