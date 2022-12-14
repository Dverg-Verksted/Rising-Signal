// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "InteractTypes.h"
#include "Game/SaveLoad/RSSavableObjectInterface.h"
#include "GameFramework/Actor.h"
#include "RSInteractStaticItemBase.generated.h"


/**
 * Base class for static interact objects.
 * When implements child class, you can use Interact and Interact_Blueprint functions that calls when character interacts with this object
 */
UCLASS(HideCategories = ("Variable", "Sockets", "Shape", "Navigation", "ComponentTick", "Physics", "Tags", "Cooking", "HLOD",
    "Mobile", "Activation", "Component Replication", "Events", "Asset User Data", "Collision", "Rendering", "Input", "Actor", "LOD"))
class RISINGSIGNAL_API ARSInteractStaticItemBase : public AActor, public IRSSavableObjectInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARSInteractStaticItemBase();

protected:
    virtual void BeginPlay() override;

public:
    // Interact with object
    virtual void Interact(ACharacter* InteractingCharacter);

    UFUNCTION(BlueprintImplementableEvent)
    void Interact_Blueprint(ACharacter* InteractingCharacter);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Interact", DisplayName = "Запрашивать предмет",
        meta = (ToolTip = "Необходимо ли проверить наличие необходимых предметов у персонажа при первом взаимодействии"), SaveGame)
    bool bNeedItem = false;

    UPROPERTY(EditAnywhere, DisplayName = "Необходимые предметы", Category = "Settings | Interact",
        meta = (EditCondition = "bNeedItem", EditConditionHides))
    TArray<FNeededItem> NeededItems;
};
