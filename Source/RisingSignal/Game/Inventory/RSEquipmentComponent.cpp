// It is owned by the company Dverg Verksted.


#include "Game/Inventory/RSEquipmentComponent.h"

#include "Library/RSFunctionLibrary.h"
#include "Player/RSGamePLayer.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"

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

    if (FirstInventorySlot.Count + SecondInventorySlot.Count > SecondInventorySlot.MaxCount)
    {
        int32 RemainingCount = FirstInventorySlot.Count + SecondInventorySlot.Count - FirstInventorySlot.MaxCount;
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

bool URSEquipmentComponent::UseItem(const FInventoryItem& Item)
{
    URSAbilitySystem* AbilitySystem = GetOwner()->FindComponentByClass<URSAbilitySystem>();
    if (Item.bCanUse && Item.InteractRowName != NAME_None)
    {
        for (auto& Effect : Item.ItemEffect)
        {
            if (Effect.EffectDuration == 0.0f)
            {
                AbilitySystem->ChangeCurrentStateValue(Effect.StateType, Effect.EffectValue);
            }
            else
            {
                AbilitySystem->AddEffect(Effect.EffectDuration,Effect.StateType,Effect.EffectValue);
            }
        }

        RemoveItem(Item);
        return true;
    }

    return false;
}

void URSEquipmentComponent::TakeInHands(int32 Index)
{
    CurrentItemInHand = Index;


    if (EquipmentSlots.Contains(CurrentItemInHand))
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
            FString::Printf(TEXT("Current item in hands is %s"), *EquipmentSlots[CurrentItemInHand].Name.ToString()));
        if(EquipmentSlots[CurrentItemInHand].bIsWeapon)
        {
            ARSBaseCharacter* PlayerCharacter = Cast<ARSBaseCharacter>(GetOwner());
            PlayerCharacter->GetWeaponComponent()->EquipWeapon(EquipmentSlots[CurrentItemInHand].WeaponSettings.WeaponClassPtr);
        }
        if(EquipmentSlots[CurrentItemInHand].bCanUse)
        {
            UseItem(EquipmentSlots[CurrentItemInHand]);
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Current slot is empty")));
    }
}
