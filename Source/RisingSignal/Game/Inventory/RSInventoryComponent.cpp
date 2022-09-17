#include "Game/Inventory/RSInventoryComponent.h"
#include "RSEquipmentComponent.h"
#include "Game/Craft/RSCraftComponent.h"
#include "Game/InteractSystem/InteractItemActor.h"


FInventoryItem::FInventoryItem(const FInventoryItem* OtherItem)
{
    InteractRowName = OtherItem->InteractRowName;
    Name = OtherItem->Name;
    Description = OtherItem->Description;
    ItemID = OtherItem->ItemID;
    TypeComponent = OtherItem->TypeComponent;
    ImageItem = OtherItem->ImageItem;
    bCanEquip = OtherItem->bCanEquip;
    bCanCraft = OtherItem->bCanCraft;
    bStack = OtherItem->bStack;
    bCanUse = OtherItem->bCanUse;
    MaxCount = OtherItem->MaxCount;
    CharacterAttributesEffects = OtherItem->CharacterAttributesEffects;
    ItemCategory = OtherItem->ItemCategory;
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

void URSInventoryComponent::RemoveItem(const FInventoryItem& InventorySlot, int32 CountRemove, bool bItemUsed)
{
    int32 IndexItem = InventorySlot.SlotIndex;

    if (InventorySlot.Count - CountRemove <= 0)
    {
        UpdateSlot(IndexItem, FInventoryItem(), 0);
    }
    else
    {
        UpdateSlot(IndexItem, InventorySlot, InventorySlot.Count - CountRemove);
    }

    if (!bItemUsed)
    {
        AInteractItemActor::SpawnItem(GetOwner(), InventorySlot, CountRemove);
    }
}

bool URSInventoryComponent::MoveItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    switch (FirstInventorySlot.TypeComponent)
    {
        case ETypeComponent::Inventory:
        {
            return MoveItemInventory(FirstInventorySlot, SecondInventorySlot);
        }
        case ETypeComponent::Craft:
        {
            return MoveItemCraft(FirstInventorySlot, SecondInventorySlot);
        }
        case ETypeComponent::Equipment:
        {
            return MoveItemEquipment(FirstInventorySlot, SecondInventorySlot);
        }
    }

    return false;
}

void URSInventoryComponent::CombineItem(
    const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    const int32 FirstIndexSlot = FirstInventorySlot.SlotIndex;
    const int32 SecondIndexSlot = SecondInventorySlot.SlotIndex;
    int32 RemainingCount = 0;

    if (FirstInventorySlot.Count + SecondInventorySlot.Count > SecondInventorySlot.MaxCount)
    {
        RemainingCount = FirstInventorySlot.Count + SecondInventorySlot.Count - FirstInventorySlot.MaxCount;
        UpdateSlot(SecondIndexSlot, SecondInventorySlot, SecondInventorySlot.MaxCount);
        UpdateSlot(FirstIndexSlot, FirstInventorySlot, RemainingCount);
        return;
    }

    UpdateSlot(SecondIndexSlot, SecondInventorySlot, SecondInventorySlot.Count + FirstInventorySlot.Count);
    UpdateSlot(FirstIndexSlot, FInventoryItem(), RemainingCount);
}

bool URSInventoryComponent::UseItem(const FInventoryItem& InventorySlot)
{
    if (InventorySlot.bCanUse && InventorySlot.ItemID != -1)
    {
        for (auto& Effect : InventorySlot.CharacterAttributesEffects)
        {
            AbilitySystem->ChangeCurrentStateValue(Effect.Key, Effect.Value);
        }

        RemoveItem(InventorySlot, 1, true);

        return true;
    }
    return false;
}

void URSInventoryComponent::AddDataItem(const FDataTableRowHandle& RowDataHandle, FName DTInteractRowName, int32 Count)
{
    FInventoryItem* ItemToAdd = FindItemData(RowDataHandle);
    ItemToAdd->InteractRowName = DTInteractRowName;
    int32 CurrentCountItem = Count;
    if (!ItemToAdd->bStack)
    {
        CurrentCountItem = 1;
    }

    if (CurrentCountItem > ItemToAdd->MaxCount)
    {
        AddStacks(ItemToAdd, CurrentCountItem);
        return;
    }

    FInventoryItem* FreeSlot = FindFreeSlot();

    if (FreeSlot == nullptr)
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
    InventoryItems[Index].TypeComponent = ETypeComponent::Inventory;

    OnInventorySlotUpdate.Broadcast(InventoryItems[Index]);
}

bool URSInventoryComponent::MoveItemInventory(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    if (SecondInventorySlot.TypeComponent == ETypeComponent::Inventory)
    {
        if (SecondInventorySlot.SlotIndex == SLOT_REMOVE)
        {
            RemoveItem(FirstInventorySlot, FirstInventorySlot.Count);
            return true;
        }

        if (FirstInventorySlot.ItemID == SecondInventorySlot.ItemID && FirstInventorySlot.SlotIndex != SecondInventorySlot.SlotIndex &&
            FirstInventorySlot.bStack)
        {
            CombineItem(FirstInventorySlot, SecondInventorySlot);
            return true;
        }

        if (FirstInventorySlot.ItemID != SecondInventorySlot.ItemID)
        {
            SwapItem(FirstInventorySlot, SecondInventorySlot);
            return true;
        }
    }
    if (SecondInventorySlot.TypeComponent == ETypeComponent::Equipment)
    {
        EquipmentComponent->EquipItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
        RemoveItem(FirstInventorySlot, FirstInventorySlot.Count, true);
        return true;
    }
    if (SecondInventorySlot.TypeComponent == ETypeComponent::Craft)
    {
        if (SecondInventorySlot.SlotIndex == OUTPUT_SLOT)
        {
            return false;
        }
        CraftComponent->AddItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
        RemoveItem(FirstInventorySlot, 1, true);
        return true;
    }

    return false;
}

bool URSInventoryComponent::MoveItemEquipment(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    if (SecondInventorySlot.TypeComponent == ETypeComponent::Inventory)
    {
        if (SecondInventorySlot.ItemID == INDEX_NONE)
        {
            EquipmentComponent->UnEquipItemFromSlot(FirstInventorySlot);
            UpdateSlot(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
            return true;
        }
        if (SecondInventorySlot.ItemID != INDEX_NONE)
        {
            EquipmentComponent->UnEquipItemFromSlot(FirstInventorySlot);
            EquipmentComponent->EquipItemInSlot(SecondInventorySlot, FirstInventorySlot.SlotIndex);
            UpdateSlot(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
            return true;
        }

        return false;
    }
    if (SecondInventorySlot.TypeComponent == ETypeComponent::Craft)
    {
        return false;
    }
    if (SecondInventorySlot.TypeComponent == ETypeComponent::Equipment)
    {
        if (SecondInventorySlot.ItemID != INDEX_NONE)
        {
            EquipmentComponent->UnEquipItemFromSlot(FirstInventorySlot);
            EquipmentComponent->EquipItemInSlot(SecondInventorySlot, FirstInventorySlot.SlotIndex);
            EquipmentComponent->UnEquipItemFromSlot(SecondInventorySlot);
            EquipmentComponent->EquipItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
            return true;
        }
        EquipmentComponent->UnEquipItemFromSlot(FirstInventorySlot);
        EquipmentComponent->EquipItemInSlot(SecondInventorySlot, FirstInventorySlot.SlotIndex);
        return true;
    }

    return false;
}

bool URSInventoryComponent::MoveItemCraft(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    if (SecondInventorySlot.SlotIndex == OUTPUT_SLOT)
    {
        return false;
    }
    if (SecondInventorySlot.TypeComponent == ETypeComponent::Inventory)
    {
        if (SecondInventorySlot.ItemID != INDEX_NONE || SecondInventorySlot.SlotIndex == SLOT_REMOVE)
        {
            return false;
        }

        CraftComponent->RemoveItemFromSlot(FirstInventorySlot);
        UpdateSlot(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
        return true;
    }
    if (SecondInventorySlot.TypeComponent == ETypeComponent::Equipment)
    {
        if (SecondInventorySlot.ItemID == INDEX_NONE)
        {
            EquipmentComponent->EquipItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
            CraftComponent->RemoveItemFromSlot(FirstInventorySlot);
            return true;
        }
        return false;
    }
    if (SecondInventorySlot.TypeComponent == ETypeComponent::Craft)
    {
        if (SecondInventorySlot.ItemID == INDEX_NONE)
        {
            CraftComponent->AddItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
            CraftComponent->RemoveItemFromSlot(FirstInventorySlot);
            return true;
        }
        return false;
    }

    return false;
}

void URSInventoryComponent::AddStacks(FInventoryItem* Item, int32 Count)
{
    int32 CurrentItemCount = Count;
    while (CurrentItemCount > 0)
    {
        FInventoryItem* FreeSlot = FindFreeSlot();
        if (FreeSlot == nullptr)
        {
            return;
        }

        if (CurrentItemCount > Item->MaxCount)
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
    for (int RowIndex = 0; RowIndex < RowNames.Num(); RowIndex++)
    {
        if (RowNames[RowIndex] == RowName)
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
    return InventoryItems.FindByPredicate([=](const FInventoryItem& Slot) { return Slot.ItemID == INDEX_NONE; });
}

void URSInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    AbilitySystem = GetOwner()->FindComponentByClass<URSAbilitySystem>();
    EquipmentComponent = GetOwner()->FindComponentByClass<URSEquipmentComponent>();
    CraftComponent = GetOwner()->FindComponentByClass<URSCraftComponent>();
    // ToDo Load Inventory
}

void URSInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

bool URSInventoryComponent::SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    UpdateSlot(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
    UpdateSlot(FirstInventorySlot.SlotIndex, SecondInventorySlot, SecondInventorySlot.Count);
    return true;
}
