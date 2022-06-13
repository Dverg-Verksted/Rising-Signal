// It is owned by the company Dverg Verksted.

#include "Game/InteractSystem/InteractComponent.h"
#include "Components/BoxComponent.h"
#include "Player/RSGamePLayer.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
    PrimaryComponentTick.bAllowTickOnDedicatedServer = false;
    PrimaryComponentTick.bTickEvenWhenPaused = false;

    this->BoxCollision = CreateDefaultSubobject<UBoxComponent>(FName("Box Collision component"));
    if (this->BoxCollision)
    {
        this->BoxCollision->SetBoxExtent(this->SizeBoxCollision);
    }
}

// Called when the game starts
void UInteractComponent::BeginPlay()
{
    Super::BeginPlay();

    this->OwnerPlayer = Cast<ARSGamePLayer>(GetOwner());
    check(this->OwnerPlayer);
}
#if WITH_EDITOR
void UInteractComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (PropertyChangedEvent.Property && PropertyChangedEvent.MemberProperty->GetName() == TEXT("SizeBoxCollision"))
    {
        if (this->BoxCollision)
        {
            this->BoxCollision->SetBoxExtent(this->SizeBoxCollision);
        }
    }
}
#endif
