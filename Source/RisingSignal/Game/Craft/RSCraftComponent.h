// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "RSCraftComponent.generated.h"

USTRUCT(BlueprintType)
struct FCraftItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Крафт",
        DisplayName="Предмет")
    FDataTableRowHandle Item;
};

USTRUCT(BlueprintType)
struct FRecipeItem : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Крафт",
        DisplayName="Необходимые ингредиенты")
    TArray<FCraftItem> RequiredIngredients;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Крафт",
        DisplayName="Выходной предмет")
    FDataTableRowHandle OutputItem;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RISINGSIGNAL_API URSCraftComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	URSCraftComponent();

    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite)
    FOnSlotChangedSignature OnCraftSlotChanged;

    void AddItemInSlot(const FInventoryItem& Item, int32 Index);
    void RemoveItemFromSlot(const FInventoryItem& Item);

protected:
	
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Настройки крафта")
	int32 MaxCraftingSlots;

private:

    void UpdateCraftSlot(int32 Index);

    UPROPERTY()
    TArray<FInventoryItem> CraftingItems;

		
};
