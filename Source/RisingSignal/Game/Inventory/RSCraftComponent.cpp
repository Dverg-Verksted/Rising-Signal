// It is owned by the company Dverg Verksted.


#include "Game/Inventory/RSCraftComponent.h"
#include "Algo/StableSort.h"
#include "Game/InteractSystem/InteractItemActor.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"


URSCraftComponent::URSCraftComponent()
{
    if (CraftingItems.Num() != MaxCraftingSlots)
    {
        for (int i = 0; i < MaxCraftingSlots; i++)
        {
            CraftingItems.Add(FInventoryItem(i));
            CraftingItems[i].TypeComponent = ETypeComponent::Craft;
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
    const int32 SlotIndex = Item.SlotIndex;
    CraftingItems[SlotIndex] = FInventoryItem(SlotIndex);
    CraftingItems[SlotIndex].TypeComponent = ETypeComponent::Craft;
    UpdateSlot(SlotIndex);
}

void URSCraftComponent::RemoveItem(const FInventoryItem& InventorySlot, int32 CountRemove, bool bItemUsed)
{
    const int32 SlotIndex = InventorySlot.SlotIndex;

    CraftingItems[SlotIndex] = FInventoryItem(SlotIndex);
    CraftingItems[SlotIndex].TypeComponent = ETypeComponent::Craft;
    UpdateSlot(SlotIndex);
    
    AInteractItemActor::SpawnItem(GetOwner(), InventorySlot, CountRemove, 2.0f);
}

void URSCraftComponent::SetCampfireNearBy(bool NewValue)
{
    bIsCampfireNearBy = NewValue;
    if(!NewValue)
    {
        ClearOutputSlot();
    }
    if(HaveAnyItems())
    {
        FindSuitableRecipe();
    }
}

void URSCraftComponent::SetSmallFireNearBy(bool NewValue)
{
    bIsSmallFireNearBy = NewValue;
    if(!NewValue)
    {
        ClearOutputSlot();
    }
    if(HaveAnyItems())
    {
        FindSuitableRecipe();
    }
}

void URSCraftComponent::SetWorkbenchNearBy(bool NewValue)
{
    bIsWorkbenchNearBy = NewValue;
    if(!NewValue)
    {
        ClearOutputSlot();
    }
    if(HaveAnyItems())
    {
        FindSuitableRecipe();
    }
}

bool URSCraftComponent::GetIsOutputSlotAvailable() const
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

    CachedBaseCharacter = Cast<ARSBaseCharacter>(GetOwner());

    CachedBaseCharacter->InventoryOpenClose.AddDynamic(this, &URSCraftComponent::ClearCraftSlots);
}

void URSCraftComponent::ClearCraftSlots()
{
    URSInventoryComponent* InventoryComponent = GetOwner()->FindComponentByClass<URSInventoryComponent>();
    for (auto CraftingItem : CraftingItems)
    {
        if(CraftingItem.InteractRowName != NAME_None)
        {
            const FInventoryItem* FreeSlot = InventoryComponent->FindFreeSlot();
            if(FreeSlot && FreeSlot->SlotIndex != SLOT_REMOVE)
            {
                InventoryComponent->MoveItem(CraftingItem, *FreeSlot);
                continue;
            }
            RemoveItem(CraftingItem, CraftingItem.Count, false);
        }
    }
}

void URSCraftComponent::UpdateSlot(int32 Index)
{
    OnCraftSlotChanged.Broadcast(CraftingItems[Index]);
}

void URSCraftComponent::PrepareItemToCraft(FDataTableRowHandle Item)
{
    FInventoryItem CraftedItem = Item.DataTable->FindRow<FInventoryItem>(Item.RowName, TEXT("Find crafted item data"));
    CraftedItem.InteractRowName = Item.RowName;
    CraftedItem.Count = 1;
    CraftedItem.SlotIndex = OUTPUT_SLOT;
    CraftedItem.TypeComponent = ETypeComponent::Craft;
    CraftingItems[OUTPUT_SLOT] = CraftedItem;
    UpdateSlot(OUTPUT_SLOT);
}

void URSCraftComponent::RefreshItems()
{
    for(int i = 0; i < CraftingItems.Num(); i++)
    {
        CraftingItems[i].bIsChecked = false;
    }
}

TArray<FName> URSCraftComponent::SortRecipesByIngredients(const UDataTable* RecipeTable) const
{
    TArray<FName> Recipes = RecipeTable->GetRowNames();
    Algo::StableSort(Recipes, [=](const FName& Recipe1, const FName& Recipe2)
    {
        const FRecipeItem* RecipeItem1 = RecipeTable->FindRow<FRecipeItem>(Recipe1, TEXT("Find recipe data"));
        const FRecipeItem* RecipeItem2 = RecipeTable->FindRow<FRecipeItem>(Recipe2, TEXT("Find recipe data"));
        return RecipeItem1->RequiredIngredients.Num() > RecipeItem2->RequiredIngredients.Num();
    });

    return Recipes;
}

bool URSCraftComponent::CanCraftRecipe(const FRecipeItem* RecipeItem) const
{
    if(RecipeItem->bIsNeedSmallFire)
    {
        if(bIsSmallFireNearBy)
        {
            return true;
        }
        else
        {
            if(RecipeItem->bIsNeedCampFire)
            {
                if(bIsCampfireNearBy)
                {
                    return true;
                }
            }
            else
            {
                if(RecipeItem->bIsNeedWorkbench)
                {
                    if(bIsWorkbenchNearBy)
                    {
                        return true;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool URSCraftComponent::HaveAnyItems() const
{
    for (auto CraftItem : CraftingItems)
    {
        if(CraftItem.InteractRowName != NAME_None)
        {
            return true;
        }
    }

    return false;
}

void URSCraftComponent::ClearOutputSlot()
{
    CraftingItems[OUTPUT_SLOT] = FInventoryItem(OUTPUT_SLOT);
    UpdateSlot(OUTPUT_SLOT);
}

void URSCraftComponent::CraftItem()
{
    RemoveUsedItems();
    SetIsOutputSlotAvailable(true);
}

void URSCraftComponent::RemoveUsedItems()
{
    for (const auto UsedItem : UsedItems)
    {
        RemoveItem((CraftingItems[UsedItem.SlotIndex]));
    }
    UsedItems.Empty();
}

void URSCraftComponent::FindSuitableRecipe()
{
    const UDataTable* RecipeDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/RisingSignal/Core/Inventory/DT_RecipeItems.DT_RecipeItems"));

    if(!RecipeDataTable)
    {
        LOG_RS(ELogRSVerb::Error, "Datatable is null");
        return;
    }

    if(CraftingItems[OUTPUT_SLOT].InteractRowName != NAME_None)
    {
        return;
    }
    
    TArray<FName> RowNames = SortRecipesByIngredients(RecipeDataTable);
    for (const auto RowName : RowNames)
    {
        const FRecipeItem* RecipeItem = RecipeDataTable->FindRow<FRecipeItem>(RowName, TEXT("Find recipe data"));
        if(!CanCraftRecipe(RecipeItem))
        {
            continue;
        }
        TArray<FCraftItem> CurrentIngredients = RecipeItem->RequiredIngredients;
        TArray<FInventoryItem> Matches;
        for (auto Ingredient : CurrentIngredients)
        {
            if(Ingredient.bIsChecked)
            {
                continue;
            }
            FInventoryItem IngredientItem = Ingredient.Item.DataTable->FindRow<FInventoryItem>(Ingredient.Item.RowName,
                TEXT("Find ingredient item data"));
            IngredientItem.InteractRowName = Ingredient.Item.RowName;
            for(FInventoryItem& CraftItem : CraftingItems)
            {
                if(CraftItem.InteractRowName != NAME_None)
                {
                    if (CraftItem == IngredientItem && !CraftItem.bIsChecked)
                    {
                        Matches.Add(CraftItem);
                        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Matches: %i"), Matches.Num()));
                        CraftItem.bIsChecked = true;
                        Ingredient.bIsChecked = true;
                        if (Matches.Num() == CurrentIngredients.Num())
                        {
                            UsedItems = Matches;
                            PrepareItemToCraft(RecipeItem->OutputItem);
                            return;
                        }
                        break;
                    }
                }
            }
        }
        Matches.Empty();
        RefreshItems();
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
