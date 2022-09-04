// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/RSInteractStaticItemBase.h"

// Sets default values
ARSInteractStaticItemBase::ARSInteractStaticItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARSInteractStaticItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARSInteractStaticItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

