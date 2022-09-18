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

    virtual void RemoveItem(const FInventoryItem& Item) override;
    virtual void UpdateSlot(int32 Index) override;
    virtual bool SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot) override;
    virtual void CombineItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot) override;

    void TakeInHands(int32 Index);

protected:
    UPROPERTY()
    TMap<int32, FInventoryItem> EquipmentSlots;

private:
    int32 CurrentItemInHand;
};
