// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "InventoryTypes.h"
#include "InventoryInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
    GENERATED_BODY()
};

class RISINGSIGNAL_API IInventoryInterface
{
    GENERATED_BODY()

public:

    virtual void CombineItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot) PURE_VIRTUAL(IInventoryInterface::CombineItem());
    virtual bool SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot) PURE_VIRTUAL(IInventoryInterface::SwapItem(), return false;);
    
    virtual void RemoveItem(const FInventoryItem& Item) PURE_VIRTUAL(IInventoryInterface::RemoveItem());
    virtual void RemoveItem(const FInventoryItem& InventorySlot, int32 CountRemove = -1, bool bItemUsed = false) PURE_VIRTUAL(IInventoryInterface::RemoveItem());
    
    virtual void UpdateSlot(int32 Index) PURE_VIRTUAL(IInventoryInterface::UpdateSlot());
    virtual void UpdateSlot(int32 Index, const FInventoryItem& Item, int32 Count) PURE_VIRTUAL(IInventoryInterface::UpdateSlot());

    virtual bool UseItem(const FInventoryItem& Item) PURE_VIRTUAL(IInventoryInterface::UseItem(), return false;);
    
};
