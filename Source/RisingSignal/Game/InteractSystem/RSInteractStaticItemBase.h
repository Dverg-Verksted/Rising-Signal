// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSInteractStaticItemBase.generated.h"


/**
 * Base class for static interact objects.
 * When implements child class, you can use Interact and Interact_Blueprint functions that calls when character interacts with this object
 */
UCLASS()
class RISINGSIGNAL_API ARSInteractStaticItemBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARSInteractStaticItemBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Interact with object
    virtual void Interact(ACharacter* InteractingCharacter);

    UFUNCTION(BlueprintImplementableEvent)
    void Interact_Blueprint(ACharacter* InteractingCharacter);
};
