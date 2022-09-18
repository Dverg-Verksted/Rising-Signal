// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "InventoryTypes.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Game/AbilitySystem/BaseComponents/RSAbilitySystem.h"
#include "RSInventoryComponent.generated.h"

class URSEquipmentComponent;
class URSCraftComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotChangedSignature, FInventoryItem, Item);

#define SLOT_REMOVE 34

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSInventoryComponent : public UActorComponent, public IInventoryInterface
{
    GENERATED_BODY()

public:
    URSInventoryComponent();

    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite)
    FOnSlotChangedSignature OnInventorySlotUpdate;

    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite)
    FOnSlotChangedSignature OnInventoryItemUse;

    UFUNCTION(BlueprintCallable, Category="Инвентарь")
    void AddDataItem(const FDataTableRowHandle& RowDataHandle, FName DTInteractRowName, int32 Count);

    UFUNCTION(BlueprintCallable, Category = "Инвентарь")
    bool MoveItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);

    UFUNCTION(BlueprintCallable, Category = "Инвентарь")
    bool UseItem(const FInventoryItem& InventorySlot);

    UFUNCTION(BlueprintCallable, Category = "Инвентарь")
    TArray<FInventoryItem> GetItems();

    UFUNCTION(BlueprintPure, Category = "Инвентарь")
    static FString ToString(FInventoryItem Item)
    {
        return FString::Printf(TEXT("Slot: %i, Count: %i"), Item.SlotIndex, Item.Count);
    }

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Инвентарь | Настройки", DisplayName = "Макс. количество предметов")
    int32 MaxCountItem = 34;

private:
    virtual void RemoveItem(const FInventoryItem& InventorySlot, int32 CountRemove = -1, bool bItemUsed = false) override;
    virtual bool SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot) override;
    virtual void CombineItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot) override;
    virtual void UpdateSlot(int32 Index, const FInventoryItem& Item, int32 Count) override;

    bool MoveItemInventory(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
    bool MoveItemEquipment(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
    bool MoveItemCraft(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);

    void AddStacks(FInventoryItem* Item, int32 Count);
    FInventoryItem* FindItemData(const FDataTableRowHandle& RowDataHandle);
    FInventoryItem* FindFreeSlot();

    TSoftObjectPtr<URSAbilitySystem> AbilitySystem;
    TSoftObjectPtr<URSEquipmentComponent> EquipmentComponent;
    TSoftObjectPtr<URSCraftComponent> CraftComponent;

    UPROPERTY()
    TArray<FInventoryItem> InventoryItems;
};
