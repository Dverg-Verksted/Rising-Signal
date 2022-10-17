// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "RSInteractStaticItemBase.generated.h"

USTRUCT()
struct FNeededItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, DisplayName = "Предмет")
    FDataTableRowHandle ItemRowHandle;

    UPROPERTY(EditAnywhere, DisplayName = "Количество")
    float ItemCount = 1;
};


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
    virtual void BeginPlay() override;

public:
    // Interact with object
    virtual void Interact(ACharacter* InteractingCharacter);

    UFUNCTION(BlueprintImplementableEvent)
    void Interact_Blueprint(ACharacter* InteractingCharacter);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Interact", DisplayName = "Запрашивать предмет",
        meta = (ToolTip = "Необходимо ли проверить наличие необходимых предметов у персонажа при первом взаимодействии"))
    bool bNeedItem = false;

    UPROPERTY(EditAnywhere, DisplayName = "Необходимые предметы", Category = "Settings | Interact",
        meta = (EditCondition = "bNeedItem", EditConditionHides))
    TArray<FNeededItem> NeededItems;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Interact", meta = (EditCondition = "bNeedItem", EditConditionHides))
    // FDataTableRowHandle ItemRowHandle;
    //
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Interact", meta = (EditCondition = "bNeedItem", EditConditionHides))
    // float ItemCount = 1;
};
