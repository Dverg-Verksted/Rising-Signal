// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/Inventory/RSInventoryItems.h"
#include "Engine/DataTable.h"
#include "RSInventoryComponent.generated.h"

// 
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
    FName RowName;
    
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventorySlotChangedSignature, EInventoryEvent, IventoryEvent, FInventoryItem, FirstItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemUseSignature, FInventoryItem, Item);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URSInventoryComponent();

    // Delegate
    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Инвентарь")
    FOnInventorySlotChangedSignature OnInventorySlotChanged;

    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Инвентарь")
    FOnInventoryItemUseSignature OnInventoryItemUse;

    UFUNCTION(BlueprintCallable, Category = "Инвентарь")
    bool ActionItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot, EInventoryEvent InventoryEvent);

    void AddItem(const FInventoryItem& Item);
    
    bool MotionItem(FInventoryItem FirstInventorySlot);
    
    bool UseItem(const FInventoryItem& FirstInventorySlot);
    
    UFUNCTION(BlueprintCallable, Category = "Инвентарь")
    TArray<FInventoryItem> GetItems();


    UFUNCTION(BlueprintPure, Category = "Инвентарь")
    static FString ToString(FInventoryItem Item)
    {
        FString Ret = "";
        Ret.Append("ItemId=");
        Ret.Append(FString::FromInt(Item.ItemID));
        Ret.Append("  Slot=");
        Ret.Append(FString::FromInt(Item.SlotIndex));
        Ret.Append("  Count=");
        Ret.Append(FString::FromInt(Item.Count));
        return Ret;
    }

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    FInventoryItem RemoveItem(const FInventoryItem& InventorySlot, int32 CountRemove = -1);
    bool DivideItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
    bool SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
    bool CombineItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot, int32& RemainingCount);
    void ChangeItem(int32 Index, const FInventoryItem& Item, int32 ChangedCount);

    UPROPERTY(EditAnywhere, Category = "Инвентарь")
    int32 MaxCountItem = 40;
    
    UPROPERTY()
    TArray<FInventoryItem> InventoryItems;
};
