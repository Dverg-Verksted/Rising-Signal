// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/Inventory/InventoryTypes.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "RSCraftComponent.generated.h"

#define OUTPUT_SLOT 6

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),
    HideCategories = ("Variable", "Transform", "Sockets", "Shape", "Navigation", "ComponentTick", "Physics", "Tags", "Cooking", "HLOD",
        "Mobile", "Activation", "Component Replication", "Events", "Asset User Data", "Collision"))
class RISINGSIGNAL_API URSCraftComponent : public UActorComponent, public IInventoryInterface
{
    GENERATED_BODY()

public:
    URSCraftComponent();

    UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
    FOnSlotChangedSignature OnCraftSlotChanged;

    void AddItemInSlot(const FInventoryItem& Item, int32 Index);
    virtual void RemoveItem(const FInventoryItem& Item) override;
    virtual bool SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot) override;
    void FindSuitableRecipe();

    UFUNCTION(BlueprintCallable)
    void CraftItem();

    UFUNCTION(BlueprintCallable)
    void SetSmallFireNearBy(bool NewValue);

    UFUNCTION(BlueprintCallable)
    void SetCampfireNearBy(bool NewValue);

    UFUNCTION(BlueprintCallable)
    void SetWorkbenchNearBy(bool NewValue);

    UFUNCTION(BlueprintGetter)
    bool GetIsOutputSlotAvailable() const;

    void SetIsOutputSlotAvailable(bool NewValue);

    void ClearOutputSlot();

    UPROPERTY()
    TArray<FInventoryItem> UsedItems;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Craft parameters")
    bool bIsSmallFireNearBy = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Craft parameters")
    bool bIsCampfireNearBy = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Craft parameters")
    bool bIsWorkbenchNearBy = false;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadOnly, Category="Настройки крафта")
    int32 MaxCraftingSlots = 7;

private:
    virtual void UpdateSlot(int32 Index) override;
    void PrepareItemToCraft(FDataTableRowHandle Item);
    void RefreshItems();
    TArray<FName> SortRecipesByIngredients(const UDataTable* RecipeTable) const;
    bool CanCraftRecipe(const FRecipeItem* RecipeItem) const;

    void RemoveUsedItems();
    
    bool bIsOutputSlotAvailable = false;

    UPROPERTY()
    TArray<FInventoryItem> CraftingItems;
};
