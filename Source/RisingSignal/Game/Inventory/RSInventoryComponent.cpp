
#include "Game/Inventory/RSInventoryComponent.h"

//#include "RSEquipmentComponent.h"
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

    if(InventorySlot.Count - CountRemove <= 0)
    {
        UpdateSlot(IndexItem, FInventoryItem(), 0);
    }
    else
    {
        UpdateSlot(IndexItem, InventorySlot, InventorySlot.Count - CountRemove);
    }

    if(!bItemUsed)
    {
        AInteractItemActor::SpawnItem(GetOwner(), InventorySlot,  CountRemove);
    }
}

bool URSInventoryComponent::MoveItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    switch (FirstInventorySlot.TypeComponent)
    {
        case ETypeComponent::Inventory:
        {
            if(SecondInventorySlot.TypeComponent == ETypeComponent::Inventory)
            {
                if(SecondInventorySlot.SlotIndex == SLOT_REMOVE)
                {
                    RemoveItem(FirstInventorySlot, FirstInventorySlot.Count);
                    return true;
                }
    
                if(FirstInventorySlot.ItemID == SecondInventorySlot.ItemID && FirstInventorySlot.SlotIndex != SecondInventorySlot.SlotIndex && FirstInventorySlot.bStack)
                {
                    CombineItem(FirstInventorySlot, SecondInventorySlot);
                    return true;
                }
    
                if(FirstInventorySlot.ItemID != SecondInventorySlot.ItemID)
                {
                    SwapItem(FirstInventorySlot, SecondInventorySlot);
                    return true;
                }
            }
            if(SecondInventorySlot.TypeComponent == ETypeComponent::Equipment && FirstInventorySlot.bCanEquip)
            {
                EquipmentComponent->EquipItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
                RemoveItem(FirstInventorySlot, FirstInventorySlot.Count, true);
                return true;
            }
            if(SecondInventorySlot.TypeComponent == ETypeComponent::Craft && FirstInventorySlot.bCanCraft)
            {
                CraftComponent->AddItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
                RemoveItem(FirstInventorySlot, 1, true);
                return true;
            }
            break;
        }
        case ETypeComponent::Craft:
        {
            if(SecondInventorySlot.TypeComponent == ETypeComponent::Inventory)
            {
                if(SecondInventorySlot.SlotIndex == SLOT_REMOVE)
                {
                    CraftComponent->RemoveItemFromSlot(FirstInventorySlot);
                    RemoveItem(FirstInventorySlot, 1, false);
                    return true;
                }
                if(SecondInventorySlot.ItemID == FirstInventorySlot.ItemID && FirstInventorySlot.bStack)
                {
                    CraftComponent->RemoveItemFromSlot(FirstInventorySlot);
                    CombineItem(FirstInventorySlot, SecondInventorySlot);
                    return true;
                }
                if(SecondInventorySlot.ItemID != INDEX_NONE)
                {
                    return false;
                }

                CraftComponent->RemoveItemFromSlot(FirstInventorySlot);
                UpdateSlot(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
                return true;
            }
            if(SecondInventorySlot.TypeComponent == ETypeComponent::Equipment)
            {
                if(SecondInventorySlot.ItemID == INDEX_NONE)
                {
                    EquipmentComponent->EquipItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
                    CraftComponent->RemoveItemFromSlot(FirstInventorySlot);
                    return true;
                }
                return false;
            }
            if(SecondInventorySlot.TypeComponent == ETypeComponent::Craft)
            {
                if(SecondInventorySlot.ItemID == INDEX_NONE)
                {
                    CraftComponent->RemoveItemFromSlot(FirstInventorySlot);
                    CraftComponent->AddItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
                    return true;
                }
                return false;
            }
            break;
        }
        case ETypeComponent::Equipment:
        {
            if(SecondInventorySlot.TypeComponent == ETypeComponent::Inventory)
            {
                if(SecondInventorySlot.ItemID == INDEX_NONE)
                {
                    EquipmentComponent->UnEquipItemFromSlot(FirstInventorySlot);
                    UpdateSlot(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
                    return true;
                }
                if(SecondInventorySlot.ItemID != INDEX_NONE && SecondInventorySlot.bCanEquip)
                {
                    EquipmentComponent->UnEquipItemFromSlot(FirstInventorySlot);
                    EquipmentComponent->EquipItemInSlot(SecondInventorySlot, FirstInventorySlot.SlotIndex);
                    UpdateSlot(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
                    return true;
                }

                return false;
            }
            if(SecondInventorySlot.TypeComponent == ETypeComponent::Craft)
            {
                return false;
            }
            if(SecondInventorySlot.TypeComponent == ETypeComponent::Equipment)
            {
                EquipmentComponent->UnEquipItemFromSlot(FirstInventorySlot);
                EquipmentComponent->EquipItemInSlot(SecondInventorySlot, FirstInventorySlot.SlotIndex);
                return true;
            }
            break;
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
    
    if(FirstInventorySlot.Count + SecondInventorySlot.Count > SecondInventorySlot.MaxCount)
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
        for(auto& Effect : InventorySlot.CharacterAttributesEffects)
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
    if(!ItemToAdd->bStack)
    {
        CurrentCountItem = 1;
    }

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
    if(Item.TypeComponent != ETypeComponent::Inventory)
    {
        return;
    }
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
