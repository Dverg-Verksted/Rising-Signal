// It is owned by the company Dverg Verksted.

#include "Game/InteractSystem/InteractItemActor.h"

// Sets default values
AInteractItemActor::AInteractItemActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	this->Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh component"));
}

// Called when the game starts or when spawned
void AInteractItemActor::BeginPlay()
{
	Super::BeginPlay();
}

#if UE_EDITOR

void AInteractItemActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	
	
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetName() == TEXT("InteractItem"))
	{
		
	}
}

#endif
