// It is owned by the company Dverg Verksted.


#include "Game/Inventory/RSEquipmentComponent.h"

#include "Library/RSFunctionLibrary.h"

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
    FInventoryItem CurrentItem = Item;
    CurrentItem.SlotIndex = Index;
    CurrentItem.TypeComponent = ETypeComponent::Equipment;
    EquipmentSlots[Index] = CurrentItem;
    UpdateSlot(Index);
}

void URSEquipmentComponent::RemoveItem(const FInventoryItem& Item)
{
    int32 SlotIndex = Item.SlotIndex;
    EquipmentSlots[SlotIndex] = FInventoryItem(SlotIndex);
    EquipmentSlots[SlotIndex].TypeComponent = ETypeComponent::Equipment;
    UpdateSlot(SlotIndex);
}

void URSEquipmentComponent::UpdateSlot(int32 Index)
{
    OnEquipmentSlotChanged.Broadcast(EquipmentSlots[Index]);
}

bool URSEquipmentComponent::SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    EquipmentSlots[FirstInventorySlot.SlotIndex] = SecondInventorySlot;
    EquipmentSlots[FirstInventorySlot.SlotIndex].SlotIndex = FirstInventorySlot.SlotIndex;
    EquipmentSlots[SecondInventorySlot.SlotIndex] = FirstInventorySlot;
    EquipmentSlots[SecondInventorySlot.SlotIndex].SlotIndex = SecondInventorySlot.SlotIndex; 
    UpdateSlot(FirstInventorySlot.SlotIndex);
    UpdateSlot(SecondInventorySlot.SlotIndex);
    
    return true;
}

void URSEquipmentComponent::CombineItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    const int32 FirstIndexSlot = FirstInventorySlot.SlotIndex;
    const int32 SecondIndexSlot = SecondInventorySlot.SlotIndex;
    int32 RemainingCount = 0;

    if (FirstInventorySlot.Count + SecondInventorySlot.Count > SecondInventorySlot.MaxCount)
    {
        RemainingCount = FirstInventorySlot.Count + SecondInventorySlot.Count - FirstInventorySlot.MaxCount;
        EquipmentSlots[SecondIndexSlot].Count = SecondInventorySlot.MaxCount;
        EquipmentSlots[FirstIndexSlot].Count = RemainingCount;
        UpdateSlot(SecondIndexSlot);
        UpdateSlot(FirstIndexSlot);
        return;
    }

    EquipmentSlots[SecondIndexSlot].Count = FirstInventorySlot.Count + SecondInventorySlot.Count;
    UpdateSlot(SecondIndexSlot);
    RemoveItem(EquipmentSlots[FirstIndexSlot]);
}

void URSEquipmentComponent::TakeInHands(int32 Index)
{
    CurrentItemInHand = Index;

    if (EquipmentSlots[CurrentItemInHand].InteractRowName != NAME_None)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
            FString::Printf(TEXT("Current item in hands is %s"), *EquipmentSlots[CurrentItemInHand].Name.ToString()));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Current slot is empty")));
    }
}
