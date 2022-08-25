// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/Inventory/RSInventoryItems.h"
#include "Engine/DataTable.h"
#include "RSInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Инвентарь",
        DisplayName="Название", meta=(ToolTip="Название предмета"))
    FText Name;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Инвентарь",
        DisplayName="Описание", meta=(ToolTip="Описание предмета", MultiLine=true))
    FText Description;
    
    UPROPERTY(BlueprintReadWrite, Category = "Инвентарь",
        DisplayName="ID предмета", meta=(ToolTip = "Идентификатор предмета. Должен быть уникальным числом."))
    int32 ItemID = -1;
    
    UPROPERTY(BlueprintReadWrite, Category = "Инвентарь",
        DisplayName="Индекс слота", meta=(ToolTip = "Параметр редактировать не нужно. По умолчанию значение всегда -1"))
    int32 SlotIndex = -1;
    
    UPROPERTY(BlueprintReadWrite, Category = "Инвентарь", DisplayName = "Количество")
    int32 Count = 0;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь",
        DisplayName = "Изображение", meta=(ToolTip = "Изображение, которое будет отображаться в слотах инвентаря"))
    UTexture2D* ImageItem;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь", DisplayName="Можно использовать в крафте?")
    bool bCanCraft = false;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь", DisplayName="Стакается предмет?")
    bool bStack = true;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь", DisplayName = "Можно использовать?")
    bool bCanUse = false;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь",
        DisplayName="Макс. количество", meta=(ToolTip = "Максимальное количество предметов в стаке", EditCondition="bStack == true", EditConditionHides))
    int32 MaxCount = 50;


    FInventoryItem() : ImageItem(nullptr) {}

    FInventoryItem(int i) : ImageItem(nullptr) { SlotIndex = i; }

    FInventoryItem(const FInventoryItem* OtherItem);

    FInventoryItem operator = (const FInventoryItem& Other)
    {
        this->Name = Other.Name;
        this->Description = Other.Description;
        this->SlotIndex = Other.SlotIndex;
        this->Count = Other.Count;
        this->ItemID = Other.ItemID;
        this->ImageItem = Other.ImageItem;
        this->bCanCraft = Other.bCanCraft;
        this->bStack = Other.bStack;
        this->bCanUse = Other.bCanUse;
        this->MaxCount = Other.MaxCount;
        return *this;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChangedSignature, FInventoryItem, Item);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URSInventoryComponent();
    
    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Инвентарь")
    FOnInventorySlotChangedSignature OnInventorySlotUpdate;

    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Инвентарь")
    FOnInventorySlotChangedSignature OnInventoryItemUse;

    UFUNCTION(BlueprintCallable, Category="Инвентарь")
    void AddDataItem(const FDataTableRowHandle& RowDataHandle, int32 Count);

    UFUNCTION(BlueprintCallable, Category = "Инвентарь")
    bool MoveItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);

    UFUNCTION(BlueprintCallable, Category = "Инвентарь")
    bool UseItem(const FInventoryItem& InventorySlot);
    
    UFUNCTION(BlueprintCallable, Category = "Инвентарь")
    TArray<FInventoryItem> GetItems();


    UFUNCTION(BlueprintPure, Category = "Инвентарь")
    static FString ToString(FInventoryItem Item)
    {
        return FString::Printf(TEXT("ItemID: %i, Slot: %i, Count: %i"), Item.ItemID, Item.SlotIndex, Item.Count);
    }

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Инвентарь | Настройки")
    int32 MaxCountItem = 35;
    
private:
    void RemoveItem(const FInventoryItem& InventorySlot, int32 CountRemove = -1);
    bool DivideItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
    bool SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
    bool CombineItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
    void UpdateSlot(int32 Index, const FInventoryItem& Item, int32 ChangedCount);

    void AddStacks(FInventoryItem* Item, int32 Count);
    FInventoryItem* FindItemData(const FDataTableRowHandle& RowDataHandle);
    FInventoryItem* FindFreeSlot();

    
    UPROPERTY()
    TArray<FInventoryItem> InventoryItems;
};
