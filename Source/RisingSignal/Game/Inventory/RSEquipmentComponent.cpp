// It is owned by the company Dverg Verksted.


#include "Game/Inventory/RSEquipmentComponent.h"

URSEquipmentComponent::URSEquipmentComponent()
{
    if (EquipmentSlots.Num() < MAX_SLOTS)
    {
        for (int i = 0; i < MAX_SLOTS; i++)
        {
            EquipmentSlots.Add(i, FInventoryItem());
        }
    }
}

void URSEquipmentComponent::EquipItemInSlot(const FInventoryItem& Item, int32 Index)
{
    UpdateEquipmentSlot(Index, Item);
}

void URSEquipmentComponent::UnEquipItemFromSlot(const FInventoryItem& Item)
{
    int32 Index = Item.SlotIndex;
    EquipmentSlots[Index] = FInventoryItem();
    UpdateEquipmentSlot(Index, EquipmentSlots[Index]);
}

void URSEquipmentComponent::TakeInHands(int32 Index)
{
    CurrentItemInHand = Index;

    if (EquipmentSlots[CurrentItemInHand].ItemID != -1)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
            FString::Printf(TEXT("Current item in hands is %s"), *EquipmentSlots[CurrentItemInHand].Name.ToString()));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Current slot is empty")));
    }
}

void URSEquipmentComponent::UpdateEquipmentSlot(int32 Index, const FInventoryItem& Item)
{
    FInventoryItem CurrentItem = Item;
    CurrentItem.SlotIndex = Index;
    CurrentItem.TypeComponent = ETypeComponent::Equipment;
    EquipmentSlots[Index] = CurrentItem;

    OnEquipmentSlotChanged.Broadcast(EquipmentSlots[Index]);
}
