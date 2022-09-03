// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/Inventory/RSInventoryItems.h"
#include "Engine/DataTable.h"
#include "Game/AbilitySystem/BaseComponents/RSAbilitySystem.h"
#include "RSInventoryComponent.generated.h"


UENUM()
enum class EItemCategory
{
    Resource UMETA(DisplayName = "Ресурс"),
    Junk UMETA(DisplayName = "Хлам")
};

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY()
    FName RowName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Инвентарь",
        DisplayName="Название", meta=(ToolTip="Название предмета"))
    FText Name;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Инвентарь",
        DisplayName="Описание", meta=(ToolTip="Описание предмета", MultiLine=true))
    FText Description;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Инвентарь",
        DisplayName="Категория", meta=(ToolTip = "Категория предмета"))
    EItemCategory ItemCategory = EItemCategory::Resource;
    
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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Инвентарь",
        DisplayName="Эффекты", meta=(ToolTip = "Эффекты накладываемые на игрока, которые изменяют значения выбранных атрибутов игрока при использовании предмета.",
        EditCondition="bCanUse", EditConditionHides))
    TMap<EAbilityStatesType, float> CharacterAttributesEffects;


    FInventoryItem() : ImageItem(nullptr) {}

    FInventoryItem(int i) : ImageItem(nullptr) { SlotIndex = i; }

    FInventoryItem(const FInventoryItem* OtherItem);

    FInventoryItem operator = (const FInventoryItem& Other)
    {
        this->RowName = Other.RowName;
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
        this->CharacterAttributesEffects = Other.CharacterAttributesEffects;
        this->ItemCategory = Other.ItemCategory;
        return *this;
    }

    bool operator == (const FInventoryItem& Other) const
    {
        return this->ItemID == Other.ItemID;
    }

    bool operator != (const FInventoryItem& Other) const
    {
        return this->ItemID != Other.ItemID;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChangedSignature, FInventoryItem, Item);

#define SLOT_REMOVE 34

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URSInventoryComponent();
    
    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite)
    FOnInventorySlotChangedSignature OnInventorySlotUpdate;

    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite)
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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Инвентарь | Настройки", DisplayName = "Макс. количество предметов")
    int32 MaxCountItem = 34;
    
private:
    void RemoveItem(const FInventoryItem& InventorySlot, int32 CountRemove = -1, bool bItemUsed = false);
    bool DivideItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
    bool SwapItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
    void CombineItem(const FInventoryItem& FirstInventorySlot, const FInventoryItem& SecondInventorySlot);
    void UpdateSlot(int32 Index, const FInventoryItem& Item, int32 ChangedCount);

    void AddStacks(FInventoryItem* Item, int32 Count);
    FInventoryItem* FindItemData(const FDataTableRowHandle& RowDataHandle);
    FInventoryItem* FindFreeSlot();
    
    TSoftObjectPtr<URSAbilitySystem> AbilitySystem;
    
    UPROPERTY()
    TArray<FInventoryItem> InventoryItems;
};
