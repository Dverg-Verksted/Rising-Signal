
#include "Game/Inventory/RSInventoryComponent.h"

URSInventoryComponent::URSInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    if (InventoryItems.Num() != MaxCountItem)
    {
        for (int i = 0; i < MaxCountItem; i++)
        {
            InventoryItems.Add(FInventoryItem(i));
        }
    }
}

TArray<FInventoryItem> URSInventoryComponent::GetItems()
{
    return InventoryItems;
}

void URSInventoryComponent::RemoveItem(const FInventoryItem& InventorySlot, int32 CountRemove)
{
    int32 IndexItem = InventorySlot.SlotIndex;

    if(InventorySlot.Count - CountRemove <= 0)
    {
        UpdateSlot(IndexItem, FInventoryItem(), 0);
    }
    else
    {
        UpdateSlot(IndexItem, InventorySlot, InventorySlot.Count - CountRemove);
    }

    //TODO spawn
}

bool URSInventoryComponent::MoveItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    if(FirstInventorySlot.ItemID == -1)
    {
        return false;
    }
    
    if(FirstInventorySlot.ItemID == SecondInventorySlot.ItemID)
    {
        if(FirstInventorySlot.bStack)
        {
            CombineItem(FirstInventorySlot, SecondInventorySlot);
        }
        else
        {
            return false;
        }
    }
    if(FirstInventorySlot.ItemID != SecondInventorySlot.ItemID)
    {
        SwapItem(FirstInventorySlot, SecondInventorySlot);
        return true;
    }
    if(SecondInventorySlot.SlotIndex == 34)
    {
        RemoveItem(FirstInventorySlot, FirstInventorySlot.Count);
        return true;
    }

    return false;
}

bool URSInventoryComponent::CombineItem(
    const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    const int32 FirstIndexSlot = FirstInventorySlot.SlotIndex;
    const int32 SecondIndexSlot = SecondInventorySlot.SlotIndex;
    int32 RemainingCount = 0;
    
    if(FirstInventorySlot.Count + SecondInventorySlot.Count > SecondInventorySlot.MaxCount)
    {
        RemainingCount = FirstInventorySlot.Count + SecondInventorySlot.Count - FirstInventorySlot.MaxCount;
        UpdateSlot(SecondIndexSlot, SecondInventorySlot, SecondInventorySlot.MaxCount);
        UpdateSlot(FirstIndexSlot, FirstInventorySlot, RemainingCount);
        return true;
    }
    
    UpdateSlot(SecondIndexSlot, SecondInventorySlot, SecondInventorySlot.Count + FirstInventorySlot.Count);
    UpdateSlot(FirstIndexSlot, FInventoryItem(), RemainingCount);
    return true;
    
}

bool URSInventoryComponent::UseItem(const FInventoryItem& InventorySlot)
{
    if (InventorySlot.bCanUse)
    {
        RemoveItem(InventorySlot, 1);
        if(OnInventoryItemUse.IsBound())
        {
            OnInventoryItemUse.Broadcast(InventorySlot);
        }
        return true;
    }
    return false;
}

void URSInventoryComponent::AddItem(const FInventoryItem& Item)
{
    int CurrentCountItem = Item.Count;
    for (int32 i = 0; i < MaxCountItem; i++)
    {
        if (InventoryItems[i].ItemID == -1)
        {
            if (CurrentCountItem > Item.MaxCount)
            {
                CurrentCountItem -= Item.MaxCount;
                UpdateSlot(i, Item, Item.MaxCount);
            }
            else
            {
                UpdateSlot(i, Item, CurrentCountItem);
                break;
            }
        }
    }
}

void URSInventoryComponent::UpdateSlot(int32 Index, const FInventoryItem& Item, int32 ChangedCount)
{
    InventoryItems[Index].ItemID = Item.ItemID;
    InventoryItems[Index].SlotIndex = Index;
    InventoryItems[Index].ImageItem = Item.ImageItem;
    InventoryItems[Index].Count = ChangedCount;
    InventoryItems[Index].bCanCraft = Item.bCanCraft;
    InventoryItems[Index].bCanUse = Item.bCanUse;
    InventoryItems[Index].MaxCount = Item.MaxCount;
    
    OnInventorySlotUpdate.Broadcast(InventoryItems[Index]);
}

void URSInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    // ToDo Load Inventory
}

void URSInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    // ToDo Save Inventory
}

bool URSInventoryComponent::DivideItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    // ToDo
    return true;
}

bool URSInventoryComponent::SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    UpdateSlot(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
    UpdateSlot(FirstInventorySlot.SlotIndex, SecondInventorySlot, SecondInventorySlot.Count);
    return true;
}
