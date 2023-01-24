// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "RSInventoryComponent.h"
#include "Components/ActorComponent.h"
#include "RSEquipmentComponent.generated.h"

#define MAX_SLOTS 4

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),
    HideCategories = ("Variable", "Transform", "Sockets", "Shape", "Navigation", "ComponentTick", "Physics", "Tags", "Cooking", "HLOD",
        "Mobile", "Activation", "Component Replication", "Events", "Asset User Data", "Collision"))
class RISINGSIGNAL_API URSEquipmentComponent : public UActorComponent, public IInventoryInterface
{
    GENERATED_BODY()

public:
    URSEquipmentComponent();

    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite)
    FOnSlotChangedSignature OnEquipmentSlotChanged;

    void EquipItemInSlot(const FInventoryItem& Item, int32 Index);

    virtual void RemoveItem(const FInventoryItem& InventorySlot, int32 CountRemove, bool bItemUsed) override;
    virtual void UpdateSlot(int32 Index) override;
    virtual bool SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot) override;
    virtual void CombineItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot) override;
    virtual bool UseItem(const FInventoryItem& Item) override;

    void TakeInHands(int32 Index);

    TMap<int32, FInventoryItem> GetItems() const { return EquipmentSlots; }
    
    UFUNCTION(BlueprintPure)
    int32 GetEquippedItem() const { return CurrentItemInHand; }

    UFUNCTION(BlueprintPure)
    TMap<int32, FInventoryItem> GetEquipments() const { return EquipmentSlots; }
    
    
    void LoadItems(TMap<int32, FInventoryItem> ItemsMap, int32 EquippedItem)
    {
        EquipmentSlots = ItemsMap;
        TakeInHands(EquippedItem);
    }

protected:
    UPROPERTY()
    TMap<int32, FInventoryItem> EquipmentSlots;

private:
    int32 CurrentItemInHand;
};
