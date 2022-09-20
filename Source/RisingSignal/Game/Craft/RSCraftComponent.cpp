// It is owned by the company Dverg Verksted.


#include "Game/Craft/RSCraftComponent.h"

URSCraftComponent::URSCraftComponent()
{
    if (CraftingItems.Num() != MaxCraftingSlots)
    {
        for (int i = 0; i < MaxCraftingSlots; i++)
        {
            CraftingItems.Add(FInventoryItem(i));
        }
    }
}

void URSCraftComponent::AddItemInSlot(const FInventoryItem& Item, int32 Index)
{
    CraftingItems[Index] = Item;
    CraftingItems[Index].Count = 1;
    CraftingItems[Index].SlotIndex = Index;
    CraftingItems[Index].TypeComponent = ETypeComponent::Craft;
    UpdateSlot(Index);

    FindSuitableRecipe();
}

void URSCraftComponent::RemoveItem(const FInventoryItem& Item)
{
    int32 SlotIndex = Item.SlotIndex;
    CraftingItems[SlotIndex] = FInventoryItem(SlotIndex);
    CraftingItems[SlotIndex].TypeComponent = ETypeComponent::Craft;
    UpdateSlot(SlotIndex);
}

void URSCraftComponent::SetCampfireNearBy(bool NewValue)
{
    bIsCampfireNearBy = NewValue;
}

void URSCraftComponent::SetWorkbenchNearBy(bool NewValue)
{
    bIsWorkbenchNearBy = NewValue;
}

bool URSCraftComponent::GetIsOutputSlotAvailable()
{
    return bIsOutputSlotAvailable;
}


void URSCraftComponent::SetIsOutputSlotAvailable(bool NewValue)
{
    bIsOutputSlotAvailable = NewValue;
}

void URSCraftComponent::BeginPlay()
{
    Super::BeginPlay();
}

void URSCraftComponent::UpdateSlot(int32 Index)
{
    OnCraftSlotChanged.Broadcast(CraftingItems[Index]);
}

void URSCraftComponent::PrepareItemToCraft(FDataTableRowHandle Item)
{
    if(CraftingItems[OUTPUT_SLOT].InteractRowName != NAME_None)
    {
        return;
    }
    
    FInventoryItem CraftedItem = Item.DataTable->FindRow<FInventoryItem>(Item.RowName, TEXT("Find crafted item data"));
    CraftedItem.InteractRowName = Item.RowName;
    CraftedItem.Count = 1;
    CraftedItem.SlotIndex = OUTPUT_SLOT;
    CraftedItem.TypeComponent = ETypeComponent::Craft;
    CraftingItems[OUTPUT_SLOT] = CraftedItem;
    UpdateSlot(OUTPUT_SLOT);
}

void URSCraftComponent::ClearOutputSlot()
{
    CraftingItems[OUTPUT_SLOT] = FInventoryItem(OUTPUT_SLOT);
    Matchs.Empty();
    UpdateSlot(OUTPUT_SLOT);
}

void URSCraftComponent::CraftItem()
{
    RemoveUsedItems();
    SetIsOutputSlotAvailable(true);
}

void URSCraftComponent::RemoveUsedItems()
{
    for (auto Match : Matchs)
    {
        RemoveItem((CraftingItems[Match.SlotIndex]));
    }
    Matchs.Empty();
    CurrentIngredients.Empty();
}

void URSCraftComponent::FindSuitableRecipe()
{
    UDataTable* RecipeDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/RisingSignal/Core/Inventory/DT_RecipeItems.DT_RecipeItems"));
    

    TArray<FName> RowNames = RecipeDataTable->GetRowNames();
    for (auto RowName : RowNames)
    {
        FRecipeItem* RecipeItem = RecipeDataTable->FindRow<FRecipeItem>(RowName, TEXT("Find recipe data"));
        if ((RecipeItem->bIsNeedCampfire && !bIsCampfireNearBy) || (RecipeItem->bIsNeedWorkbench && !bIsWorkbenchNearBy))
        {
            return;
        }
        if(CurrentIngredients.Num() == 0)
        {
            CurrentIngredients = RecipeItem->RequiredIngredients;
        }
        for (auto Ingredient : CurrentIngredients)
        {
            if(Ingredient.bIsChecked)
            {
                continue;
            }
            FInventoryItem IngredientItem = Ingredient.Item.DataTable->FindRow<FInventoryItem>(Ingredient.Item.RowName,
                TEXT("Find ingredient item data"));
            for (auto CraftItem : CraftingItems)
            {
                if (CraftItem == IngredientItem)
                {
                    Matchs.Add(CraftItem);
                    Ingredient.bIsChecked = true;
                    break;
                }
                if (Matchs.Num() == CurrentIngredients.Num())
                {
                    PrepareItemToCraft(RecipeItem->OutputItem);
                    return;
                }
            }
        }
        CurrentIngredients.Empty();
    }
}

bool URSCraftComponent::SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    CraftingItems[FirstInventorySlot.SlotIndex] = SecondInventorySlot;
    CraftingItems[FirstInventorySlot.SlotIndex].SlotIndex = FirstInventorySlot.SlotIndex;
    CraftingItems[SecondInventorySlot.SlotIndex] = FirstInventorySlot;
    CraftingItems[SecondInventorySlot.SlotIndex].SlotIndex = SecondInventorySlot.SlotIndex;
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RowName FirstSlot, RowName SecondSlot %s, %s"), *FirstInventorySlot.InteractRowName.ToString(), *SecondInventorySlot.InteractRowName.ToString()));
    UpdateSlot(SecondInventorySlot.SlotIndex);
    UpdateSlot(FirstInventorySlot.SlotIndex);
    return true;
}
