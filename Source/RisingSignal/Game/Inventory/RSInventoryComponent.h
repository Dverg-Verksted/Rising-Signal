// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/Inventory/RSInventoryItems.h"
#include "Engine/DataTable.h"
#include "RSInventoryComponent.generated.h"

UENUM()
enum EInventoryEvent
{
	Remove,
	Divide,
	Motion,
	Use,
	Warning,
	Changed
};

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь")
	int32 Count = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь")
	int32 SlotIndex = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь")
	int32 ItemID = -1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь")
	UTexture2D* ImageItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь")
	bool bCanCraft = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь")
	bool bStack = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь")
	bool bCanUse = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь")
	int32 MaxCount = 50;

	FInventoryItem() : ImageItem(nullptr) {}

	FInventoryItem(int i) : ImageItem(nullptr) { SlotIndex = i; }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryEvent, EInventoryEvent, IventoryEvent, FInventoryItem, FirstItem);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URSInventoryComponent();

	// Delegate
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Инвентарь")
	FOnInventoryEvent OnInventoryEvent;

	UFUNCTION(BlueprintCallable, Category = "Инвентарь")
	bool ActionItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot, EInventoryEvent InventoryEvent);

	UFUNCTION(BlueprintCallable, Category = "Инвентарь")
	TArray<FInventoryItem> GetItems();

	UPROPERTY(EditAnywhere, Category = "Инвентарь")
	int32 MaxCountItem = 40;

	UFUNCTION(BlueprintPure, Category = "Инвентарь")
	static FString ToString(FInventoryItem item)
	{
		FString ret = "";
		ret.Append("itemId=");
		ret.Append(FString::FromInt(item.ItemID));
		ret.Append("  Slot=");
		ret.Append(FString::FromInt(item.SlotIndex));
		ret.Append("  Count=");
		ret.Append(FString::FromInt(item.Count));
		return ret;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FInventoryItem RemoveItem(const FInventoryItem& FirstInventorySlot, int32 CountRemove = -1);
	bool DivideItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
	bool SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
	bool MotionItem(FInventoryItem FirstInventorySlot);
	bool CombineItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot, int32& RemainingCount);
	bool UseItem(const FInventoryItem& FirstInventorySlot);
	void ChangeItem(int32 Index, const FInventoryItem& Item, int32 ChangedCount);
	void InsertItem(const FInventoryItem& Item);

	UPROPERTY()
	TArray<FInventoryItem> InventoryBox;
};
