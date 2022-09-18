// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/Inventory/InventoryTypes.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "RSCraftComponent.generated.h"

#define OUTPUT_SLOT 6

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSCraftComponent : public UActorComponent, public IInventoryInterface
{
    GENERATED_BODY()

public:
    URSCraftComponent();

    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite)
    FOnSlotChangedSignature OnCraftSlotChanged;

    void AddItemInSlot(const FInventoryItem& Item, int32 Index);
    virtual void RemoveItem(const FInventoryItem& Item) override;
    virtual bool SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot) override;

    void SetCampfireNearBy(bool NewValue);
    void SetWorkbenchNearBy(bool NewValue);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Настройки крафта")
    int32 MaxCraftingSlots = 7;

private:
    virtual void UpdateSlot(int32 Index) override;

    bool bIsCampfireNearBy = false;
    bool bIsWorkbenchNearBy = false;

    UPROPERTY()
    TArray<FInventoryItem> CraftingItems;
};
