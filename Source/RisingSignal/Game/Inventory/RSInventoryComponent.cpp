#include "Game/Inventory/RSInventoryComponent.h"
#include "RSEquipmentComponent.h"
#include "Game/Craft/RSCraftComponent.h"
#include "Game/InteractSystem/InteractItemActor.h"


FInventoryItem::FInventoryItem(const FInventoryItem* OtherItem)
{
    InteractRowName = OtherItem->InteractRowName;
    Name = OtherItem->Name;
    Description = OtherItem->Description;
    CostDurability = OtherItem->CostDurability;
    TypeComponent = OtherItem->TypeComponent;
    ImageItem = OtherItem->ImageItem;
    bCanEquip = OtherItem->bCanEquip;
    bCanCraft = OtherItem->bCanCraft;
    bStack = OtherItem->bStack;
    bCanUse = OtherItem->bCanUse;
    MaxCount = OtherItem->MaxCount;
    CharacterAttributesEffects = OtherItem->CharacterAttributesEffects;
    ItemCategory = OtherItem->ItemCategory;
    ItemsDurability = OtherItem->ItemsDurability;
    bIsChecked = OtherItem->bIsChecked;
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
    const int32 IndexItem = InventorySlot.SlotIndex;

    if (InventorySlot.Count - CountRemove <= 0)
    {
        UpdateSlot(IndexItem, FInventoryItem(IndexItem), 0);
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
    if (InventorySlot.bCanUse && InventorySlot.InteractRowName != NAME_None)
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

    const FInventoryItem* FreeSlot = FindFreeSlot();

    if (FreeSlot == nullptr)
    {
        return;
    }

    UpdateSlot(FreeSlot->SlotIndex, ItemToAdd, CurrentCountItem);
}

void URSInventoryComponent::UpdateSlot(int32 Index, const FInventoryItem& Item, int32 Count)
{
    FInventoryItem CurrentItem = Item;
    CurrentItem.SlotIndex = Index;
    CurrentItem.Count = Count;
    InventoryItems[Index] = CurrentItem;
    InventoryItems[Index].TypeComponent = ETypeComponent::Inventory;

    OnInventorySlotUpdate.Broadcast(InventoryItems[Index]);
}

bool URSInventoryComponent::MoveItemInventory(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    switch (SecondInventorySlot.TypeComponent)
    {
        case ETypeComponent::Inventory:
        {
            if (SecondInventorySlot.SlotIndex == SLOT_REMOVE)
            {
                RemoveItem(FirstInventorySlot, FirstInventorySlot.Count);
                return true;
            }

            if (FirstInventorySlot.InteractRowName == SecondInventorySlot.InteractRowName && FirstInventorySlot.SlotIndex != SecondInventorySlot
                .SlotIndex &&
                FirstInventorySlot.bStack)
            {
                CombineItem(FirstInventorySlot, SecondInventorySlot);
                return true;
            }

            if (FirstInventorySlot.InteractRowName != SecondInventorySlot.InteractRowName || SecondInventorySlot.InteractRowName == NAME_None)
            {
                SwapItem(FirstInventorySlot, SecondInventorySlot);
                return true;
            }
            return false;
        }
        case ETypeComponent::Equipment:
        {
            if (SecondInventorySlot.InteractRowName == NAME_None)
            {
                EquipmentComponent->EquipItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
                RemoveItem(FirstInventorySlot, FirstInventorySlot.Count, true);
                return true;
            }
            return false;
        }
        case ETypeComponent::Craft:
        {
            if (SecondInventorySlot.SlotIndex == OUTPUT_SLOT)
            {
                return false;
            }
            CraftComponent->AddItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
            RemoveItem(FirstInventorySlot, 1, true);
            return true;
        }
    }
    
    return false;
}

bool URSInventoryComponent::MoveItemEquipment(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    switch (SecondInventorySlot.TypeComponent)
    {
        case ETypeComponent::Inventory:
        {
            if (SecondInventorySlot.InteractRowName == NAME_None)
            {
                EquipmentComponent->RemoveItem(FirstInventorySlot);
                UpdateSlot(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
                return true;
            }
            if (SecondInventorySlot.InteractRowName != NAME_None)
            {
                EquipmentComponent->RemoveItem(FirstInventorySlot);
                EquipmentComponent->EquipItemInSlot(SecondInventorySlot, FirstInventorySlot.SlotIndex);
                UpdateSlot(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
                return true;
            }

            return false;
        }
        case ETypeComponent::Equipment:
        {
            if (SecondInventorySlot.InteractRowName != NAME_None)
            {
                if (SecondInventorySlot.InteractRowName == FirstInventorySlot.InteractRowName && FirstInventorySlot.bStack)
                {
                    EquipmentComponent->CombineItem(FirstInventorySlot, SecondInventorySlot);
                    return true;
                }
                EquipmentComponent->SwapItem(FirstInventorySlot, SecondInventorySlot);
                return true;
            }
            EquipmentComponent->EquipItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
            EquipmentComponent->RemoveItem(FirstInventorySlot);
            return true;
        }
        case ETypeComponent::Craft:
        {
            return false;
        }
    }

    return false;
}

bool URSInventoryComponent::MoveItemCraft(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    switch (SecondInventorySlot.TypeComponent)
    {
        case ETypeComponent::Inventory:
        {
            if (SecondInventorySlot.InteractRowName != NAME_None || SecondInventorySlot.SlotIndex == SLOT_REMOVE)
            {
                return false;
            }
            if (FirstInventorySlot.SlotIndex == OUTPUT_SLOT)
            {
                if (!CraftComponent->GetIsOutputSlotAvailable())
                {
                    return false;
                }
                CraftComponent->SetIsOutputSlotAvailable(false);
                CraftComponent->FindSuitableRecipe();
            }

            CraftComponent->RemoveItem(FirstInventorySlot);
            UpdateSlot(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
            
            for (auto Item : CraftComponent->UsedItems)
            {
                if (FirstInventorySlot == Item)
                {
                    CraftComponent->ClearOutputSlot();
                    CraftComponent->FindSuitableRecipe();
                    break;
                }
            }
            
            return true;
        }
        case ETypeComponent::Equipment:
        {
            if (SecondInventorySlot.InteractRowName == NAME_None)
            {
                EquipmentComponent->EquipItemInSlot(FirstInventorySlot, SecondInventorySlot.SlotIndex);
                CraftComponent->RemoveItem(FirstInventorySlot);
                return true;
            }
            return false;
        }
        case ETypeComponent::Craft:
        {
            if (SecondInventorySlot.SlotIndex == OUTPUT_SLOT)
            {
                return false;
            }

            CraftComponent->SwapItem(FirstInventorySlot, SecondInventorySlot);
            return true;
        }
    }

    return false;
}

void URSInventoryComponent::AddStacks(FInventoryItem* Item, int32 Count)
{
    int32 CurrentItemCount = Count;
    while (CurrentItemCount > 0)
    {
        const FInventoryItem* FreeSlot = FindFreeSlot();
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
    const FName RowName = RowDataHandle.RowName;
    FInventoryItem* Item = DataTable->FindRow<FInventoryItem>(RowName, TEXT("Find item data"));

    return Item;
}

FInventoryItem* URSInventoryComponent::FindFreeSlot()
{
    return InventoryItems.FindByPredicate([=](const FInventoryItem& Slot) { return Slot.InteractRowName == NAME_None; });
}

bool URSInventoryComponent::FindItem(FName RowName, int32 Count)
{
    const FInventoryItem* Item = InventoryItems.FindByPredicate([=](const FInventoryItem& Slot) {return Slot.InteractRowName == RowName && Slot.Count == Count; });
    if(Item)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Item found")));
        RemoveItem(*Item, Item->Count, true);
        return true;
    }
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Item not found")));
    return false;
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
