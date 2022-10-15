// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/RSInteractStaticItemBase.h"

#include "GameFramework/Character.h"
#include "Library/RSFunctionLibrary.h"

// Sets default values
ARSInteractStaticItemBase::ARSInteractStaticItemBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
}


void ARSInteractStaticItemBase::BeginPlay()
{
    Super::BeginPlay();
}

void ARSInteractStaticItemBase::Interact(ACharacter* InteractingCharacter)
{
    LOG_RS(ELogRSVerb::Display, InteractingCharacter->GetName() + " interacted with " + GetName());
    Interact_Blueprint(InteractingCharacter);
}
