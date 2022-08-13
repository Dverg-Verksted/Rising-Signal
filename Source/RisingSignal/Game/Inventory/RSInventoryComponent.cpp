
#include "Game/Inventory/RSInventoryComponent.h"

URSInventoryComponent::URSInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    if (InventoryItems.Num() != MaxCountItem)
    {
        for (int i = 0; i < MaxCountItem; i++)
        {
            FInventoryItem Item;
            Item.SlotIndex = i;
            InventoryItems.Add(FInventoryItem(i));
        }
    }
}

bool URSInventoryComponent::ActionItem(
    const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot, EInventoryEvent InventoryEvent)
{
    switch (InventoryEvent)
    {
        case Motion:
        {
            if ((SecondInventorySlot.ItemID == -1) && (SecondInventorySlot.SlotIndex == -1))
            {
                MotionItem(FirstInventorySlot);
            }
            else
            {
                int32 Count = FirstInventorySlot.Count;
                CombineItem(FirstInventorySlot, SecondInventorySlot, Count);
            }
            break;
        }
        case Use:
        {
            const auto Item = UseItem(FirstInventorySlot);
            break;
        }
        case Remove:
        {
            const auto Item = RemoveItem(FirstInventorySlot, -1);
            OnInventorySlotChanged.Broadcast(Remove, Item);
            break;
        }
        case Divide:
        {
            break;
        }
        default:
        {
            OnInventorySlotChanged.Broadcast(InventoryEvent, FirstInventorySlot);
            return false;
        };
    }
    return true;
}

TArray<FInventoryItem> URSInventoryComponent::GetItems()
{
    return InventoryItems;
}

FInventoryItem URSInventoryComponent::RemoveItem(const FInventoryItem& InventorySlot, int32 CountRemove)
{
    int32 IndexItem = InventorySlot.SlotIndex;
    if (IndexItem == -1)
    {
        return FInventoryItem();
    }
    FInventoryItem Item = InventorySlot;

    if (CountRemove == -1)
    {
        ChangeItem(IndexItem, FInventoryItem(), 0);
        // ToDo spawn
    }
    else
    {
        if ((InventorySlot.Count - CountRemove) <= 0)
        {
            ChangeItem(IndexItem, FInventoryItem(), 0);
            Item.Count = InventorySlot.Count;
        }
        else
        {
            ChangeItem(IndexItem, InventorySlot, InventorySlot.Count - CountRemove);
            Item.Count = CountRemove;
        }
    }
    return Item;
}

bool URSInventoryComponent::MotionItem(FInventoryItem FirstInventorySlot)
{
    /*bool bCombined = false;
    int32 RemainingCount = 0;
    for (int32 i = 0; i < MaxCountItem; i++)
    {
        if ((FirstInventorySlot.bStack) && (FirstInventorySlot.ItemID == InventoryItems[i].ItemID) &&
            (InventoryItems[i].Count != InventoryItems[i].MaxCount) && (FirstInventorySlot.SlotIndex == -1))
        {
            CombineItem(FirstInventorySlot, InventoryItems[i], RemainingCount);
            FirstInventorySlot.Count = RemainingCount;
            bCombined = true;
            if (RemainingCount != 0)
            {
                continue;
            }
            break;
        }
    }
    if (bCombined && (RemainingCount != 0))
    {
        InsertItem(FirstInventorySlot);
        if (FirstInventorySlot.Count != 0)
        {
            OnInventorySlotChanged.Broadcast(Remove, FirstInventorySlot);
        }
        return true;
    }
    if (bCombined && (RemainingCount == 0))
    {
        return true;
    }
    if (!bCombined && (RemainingCount == 0))
    {
        InsertItem(FirstInventorySlot);
        return true;
    }
    return false;*/
    return false;
}

bool URSInventoryComponent::CombineItem(
    const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot, int32& RemainingCount)
{
    const int32 FirstIndexItem = FirstInventorySlot.SlotIndex;
    const int32 SecondIndexItem = SecondInventorySlot.SlotIndex;
    if (FirstInventorySlot.bStack && (FirstInventorySlot.ItemID == SecondInventorySlot.ItemID))
    {
        if ((FirstInventorySlot.Count + SecondInventorySlot.Count) > FirstInventorySlot.MaxCount)
        {
            RemainingCount = FirstInventorySlot.Count + SecondInventorySlot.Count - FirstInventorySlot.MaxCount;
            ChangeItem(SecondIndexItem, SecondInventorySlot, SecondInventorySlot.MaxCount);
            ChangeItem(FirstIndexItem, FirstInventorySlot, RemainingCount);
        }
        else
        {
            RemainingCount = 0;
            ChangeItem(SecondIndexItem, SecondInventorySlot, SecondInventorySlot.Count + FirstInventorySlot.Count);
            ChangeItem(FirstIndexItem, FirstInventorySlot, RemainingCount);
        }
        return true;
    }

    SwapItem(FirstInventorySlot, SecondInventorySlot);
    return false;
}

bool URSInventoryComponent::UseItem(const FInventoryItem& FirstInventorySlot)
{
    if (FirstInventorySlot.bCanUse)
    {
        const auto Item = RemoveItem(FirstInventorySlot, 1);
        if(OnInventoryItemUse.IsBound())
        {
            OnInventoryItemUse.Broadcast(Item);
        }
        return true;
    }
    return false;
}

void URSInventoryComponent::AddItem(const FInventoryItem& Item)
{
    int CurrentCountItem = Item.Count;
    for (int32 i = 0; i < MaxCountItem; i++)
    {
        if (InventoryItems[i].ItemID == -1)
        {
            if (CurrentCountItem > Item.MaxCount)
            {
                CurrentCountItem -= Item.MaxCount;
                ChangeItem(i, Item, Item.MaxCount);
            }
            else
            {
                ChangeItem(i, Item, CurrentCountItem);
                break;
            }
        }
    }
}

void URSInventoryComponent::ChangeItem(int32 Index, const FInventoryItem& Item, int32 ChangedCount)
{
    if (Index == -1)
    {
        return;
    }
    InventoryItems[Index].Count = ChangedCount <= Item.MaxCount ? ChangedCount : Item.MaxCount;
    InventoryItems[Index].bCanCraft = Item.bCanCraft;
    InventoryItems[Index].bCanUse = Item.bCanUse;

    InventoryItems[Index].MaxCount = Item.MaxCount;
    if (InventoryItems[Index].Count <= 0)
    {
        InventoryItems[Index].ItemID = -1;
        InventoryItems[Index].ImageItem = nullptr;
    }
    else
    {
        InventoryItems[Index].ImageItem = Item.ImageItem;
        InventoryItems[Index].ItemID = Item.ItemID;
    }
    InventoryItems[Index].SlotIndex = Index;
    OnInventorySlotChanged.Broadcast(Changed, InventoryItems[Index]);
}

void URSInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    // ToDo Load Inventory
}

void URSInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    // ToDo Save Inventory
}

bool URSInventoryComponent::DivideItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    // ToDo
    return true;
}

bool URSInventoryComponent::SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot)
{
    ChangeItem(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
    ChangeItem(FirstInventorySlot.SlotIndex, SecondInventorySlot, SecondInventorySlot.Count);
    return true;
}
