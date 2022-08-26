
#include "Game/Inventory/RSInventoryComponent.h"

FInventoryItem::FInventoryItem(const FInventoryItem* OtherItem)
{
    Name = OtherItem->Name;
    Description = OtherItem->Description;
    ItemID = OtherItem->ItemID;
    ImageItem = OtherItem->ImageItem;
    bCanCraft = OtherItem->bCanCraft;
    bStack = OtherItem->bStack;
    bCanUse = OtherItem->bCanUse;
    MaxCount = OtherItem->MaxCount;
}

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
    if(SecondInventorySlot.SlotIndex == 34)
    {
        RemoveItem(FirstInventorySlot, FirstInventorySlot.Count);
        return true;
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

void URSInventoryComponent::AddDataItem(const FDataTableRowHandle& RowDataHandle, int32 Count)
{
    FInventoryItem* ItemToAdd = FindItemData(RowDataHandle);
    int32 CurrentCountItem = Count;

    if(CurrentCountItem > ItemToAdd->MaxCount)
    {
        AddStacks(ItemToAdd, CurrentCountItem);
        return;
    }

    FInventoryItem* FreeSlot = FindFreeSlot();

    if(FreeSlot == nullptr)
    {
        return;
    }
    
    UpdateSlot(FreeSlot->SlotIndex, ItemToAdd, CurrentCountItem);
}

void URSInventoryComponent::UpdateSlot(int32 Index, const FInventoryItem& Item, int32 ChangedCount)
{
    FInventoryItem CurrentItem = Item;
    CurrentItem.SlotIndex = Index;
    CurrentItem.Count = ChangedCount;
    InventoryItems[Index] = CurrentItem;
    
    OnInventorySlotUpdate.Broadcast(InventoryItems[Index]);
}

void URSInventoryComponent::AddStacks(FInventoryItem* Item, int32 Count)
{
    int32 CurrentItemCount = Count;
    while(CurrentItemCount > 0)
    {
        FInventoryItem* FreeSlot = FindFreeSlot();
        if(FreeSlot == nullptr)
        {
            return;
        }

        if(CurrentItemCount > Item->MaxCount)
        {
            CurrentItemCount -= Item->MaxCount;
            UpdateSlot(FreeSlot->SlotIndex, Item, Item->MaxCount);
        }
        else
        {
            UpdateSlot(FreeSlot->SlotIndex, Item, CurrentItemCount);
            CurrentItemCount = 0;
        }
    }
}

FInventoryItem* URSInventoryComponent::FindItemData(const FDataTableRowHandle& RowDataHandle)
{
    const UDataTable* DataTable = RowDataHandle.DataTable;
    FName RowName = RowDataHandle.RowName;
    FInventoryItem* Item = DataTable->FindRow<FInventoryItem>(RowName, TEXT("Find item data"));
    
    TArray<FName> RowNames = DataTable->GetRowNames();
    for(int RowIndex = 0; RowIndex < RowNames.Num(); RowIndex++)
    {
        if(RowNames[RowIndex] == RowName)
        {
            // Index in Data Table starts at 1, so need increment RowIndex for exact match to index in Data Table.
            ++RowIndex;
            
            Item->ItemID = RowIndex;
        }
    }

    return Item;
}

FInventoryItem* URSInventoryComponent::FindFreeSlot()
{
    return InventoryItems.FindByPredicate([=](const FInventoryItem& Slot) { return Slot.ItemID == -1; });
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
