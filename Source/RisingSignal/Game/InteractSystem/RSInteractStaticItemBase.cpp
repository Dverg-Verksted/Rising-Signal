// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/RSInteractStaticItemBase.h"

#include "Game/Inventory/RSInventoryComponent.h"
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
    if (bNeedItem)
    {
        if (const auto InvComp = InteractingCharacter->FindComponentByClass<URSInventoryComponent>())
        {
            if (!InvComp->FindItemsToUse(NeededItems))
            {
                bNeedItem = true;
                return;
            }

            bNeedItem = false;
        }
    }

    Interact_Blueprint(InteractingCharacter);
}
