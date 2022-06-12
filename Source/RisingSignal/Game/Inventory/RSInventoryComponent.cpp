// It is owned by the company Dverg Verksted.

#include "Game/Inventory/RSInventoryComponent.h"

// Sets default values for this component's properties
URSInventoryComponent::URSInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	if (InventoryBox.Num() != MaxCountItem)
	{
		for (int i = 0; i < MaxCountItem; i++)
		{
			FInventoryItem Item;
			Item.SlotIndex = i;
			InventoryBox.Add(FInventoryItem(i));
		}
	}

	// ...
}

bool URSInventoryComponent::ActionItem(
	const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot, EInventoryEvent InventoryEvent)
{
	switch (InventoryEvent)
	{
		case Motion:
		{
			if (SecondInventorySlot.ItemID == -1)
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
			OnInventoryEvent.Broadcast(Remove, Item);
			break;
		}
		case Divide:
		{
			break;
		}
		default:
		{
			OnInventoryEvent.Broadcast(InventoryEvent, FirstInventorySlot);
			return false;
		};
	}
	return true;
}

TArray<FInventoryItem> URSInventoryComponent::GetItems()
{
	return InventoryBox;
}

FInventoryItem URSInventoryComponent::RemoveItem(const FInventoryItem& FirstInventorySlot, int32 CountRemove)
{
	int32 IndexItem = FirstInventorySlot.SlotIndex;
	if (IndexItem == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("элемент не найден для удаления, id= : %d!"), FirstInventorySlot.ItemID);
		return FInventoryItem();
	}
	UE_LOG(LogTemp, Warning, TEXT("элемент найден для удаления, id= : %d! количество = %d"), FirstInventorySlot.ItemID, CountRemove);
	FInventoryItem Item = FirstInventorySlot;

	if (CountRemove == -1)
	{
		ChangeItem(IndexItem, FInventoryItem(), 0);
		// ToDo spawn
	}
	else
	{
		if ((FirstInventorySlot.Count - CountRemove) <= 0)
		{
			ChangeItem(IndexItem, FInventoryItem(), 0);
			Item.Count = FirstInventorySlot.Count;
		}
		else
		{
			ChangeItem(IndexItem, FirstInventorySlot, FirstInventorySlot.Count - CountRemove);
			Item.Count = CountRemove;
		}
	}
	return Item;
}

bool URSInventoryComponent::MotionItem(FInventoryItem FirstInventorySlot)
{
	bool bCombined = false;
	int32 RemainingCount = 0;
	for (int32 i = 0; i < MaxCountItem; i++)
	{
		if ((FirstInventorySlot.bStack) && (FirstInventorySlot.ItemID == InventoryBox[i].ItemID) &&
			(InventoryBox[i].Count != InventoryBox[i].MaxCount) && (FirstInventorySlot.SlotIndex == -1))
		{
			CombineItem(FirstInventorySlot, InventoryBox[i], RemainingCount);
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
			OnInventoryEvent.Broadcast(Remove, FirstInventorySlot);
		}
		return true;
	}
	else if (bCombined && (RemainingCount == 0))
	{
		// OnInventoryEvent.Broadcast(Motion, FirstInventorySlot);
		return true;
	}
	else if (!bCombined && (RemainingCount == 0))
	{
		InsertItem(FirstInventorySlot);
		// if (FirstInventorySlot.Count != 0)
		// {
		// OnInventoryEvent.Broadcast(Remove, FirstInventorySlot);
		// }
		// OnInventoryEvent.Broadcast(Motion, FirstInventorySlot);
		return true;
	}
	return false;
}

bool URSInventoryComponent::CombineItem(
	const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot, int32& RemainingCount)
{
	int32 FirstIndexItem = FirstInventorySlot.SlotIndex;
	int32 SecondIndexItem = SecondInventorySlot.SlotIndex;
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
	else
	{
		SwapItem(FirstInventorySlot, SecondInventorySlot);
	}
	return false;
}

bool URSInventoryComponent::UseItem(const FInventoryItem& FirstInventorySlot)
{
	if (FirstInventorySlot.bCanUse)
	{
		auto Item = RemoveItem(FirstInventorySlot, 1);
		OnInventoryEvent.Broadcast(Use, Item);
		return true;
	}
	return false;
}

void URSInventoryComponent::InsertItem(const FInventoryItem& Item)
{
	int CurrentCountItem = Item.Count;
	for (int32 i = 0; i < MaxCountItem; i++)
	{
		if (InventoryBox[i].ItemID == -1)
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
	InventoryBox[Index].Count = ChangedCount <= Item.MaxCount ? ChangedCount : Item.MaxCount;
	InventoryBox[Index].bCanCraft = Item.bCanCraft;
	InventoryBox[Index].bCanUse = Item.bCanUse;

	InventoryBox[Index].MaxCount = Item.MaxCount;
	if (InventoryBox[Index].Count <= 0)
	{
		InventoryBox[Index].ItemID = -1;
		InventoryBox[Index].ImageItem = nullptr;
	}
	else
	{
		InventoryBox[Index].ImageItem = Item.ImageItem;
		InventoryBox[Index].ItemID = Item.ItemID;
	}
	InventoryBox[Index].SlotIndex = Index;
	OnInventoryEvent.Broadcast(Changed, InventoryBox[Index]);
}

// Called when the game starts
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
	// ToDo
	ChangeItem(SecondInventorySlot.SlotIndex, FirstInventorySlot, FirstInventorySlot.Count);
	ChangeItem(FirstInventorySlot.SlotIndex, SecondInventorySlot, SecondInventorySlot.Count);
	return true;
}
