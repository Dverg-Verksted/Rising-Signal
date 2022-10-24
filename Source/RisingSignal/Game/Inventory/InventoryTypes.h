#pragma once
#include "Engine/DataTable.h"
#include "Game/AbilitySystem/BaseComponents/RSAbilitySystem.h"

#include "InventoryTypes.generated.h"

UENUM()
enum class EItemCategory
{
    Resource UMETA(DisplayName = "Ресурс"),
    Junk UMETA(DisplayName = "Хлам"),
    Weapon UMETA(DisplayName = "Оружие"),
    Food UMETA(DisplayName = "Еда"),
    Tools UMETA(DisplayName = "Инструменты")
};

UENUM(BlueprintType)
enum class ETypeComponent : uint8
{
    Inventory,
    Equipment,
    Craft
};

USTRUCT(BlueprintType)
struct FItemEffect
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Эффект",
        DisplayName="Тип")
    EAbilityStatesType StateType;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Эффект",
        DisplayName="Значение эффекта",
        meta=(ToolTip = "Значение, которое будет накладываться на выбранный тип."))
    float EffectValue;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Эффект",
        DisplayName="Длительность эффекта",
        meta=(ToolTip = "Длительность эффекта, при которой будет накладываться эффект", ClampMin = 0.0f, UIMin = 0.0f))
    float EffectDuration = 0.0f;
};

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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Инвентарь",
        DisplayName="Категория", meta=(ToolTip = "Категория предмета"))
    EItemCategory ItemCategory = EItemCategory::Resource;

    UPROPERTY(BlueprintReadWrite, Category = "Инвентарь",
        DisplayName="Индекс слота", meta=(ToolTip = "Параметр редактировать не нужно. По умолчанию значение всегда -1"))
    int32 SlotIndex = -1;

    UPROPERTY(BlueprintReadWrite, Category="Инвентарь",
        DisplayName="Тип компонента", meta=(ToolTip = "Тип компонента, к которому будет принадлежать данный слот"))
    ETypeComponent TypeComponent = ETypeComponent::Inventory;

    UPROPERTY(BlueprintReadWrite, Category = "Инвентарь", DisplayName = "Количество")
    int32 Count = 0;

    UPROPERTY(BlueprintReadWrite, Category= "Инвентарь",
        DisplayName = "Трата прочности", meta=(ToolTip = "Трата прочности при использовании предмета", ClampMin = 0.0f, UIMin = 0.0f))
    float CostDurability = 0.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь",
        DisplayName = "Изображение", meta=(ToolTip = "Изображение, которое будет отображаться в слотах инвентаря"))
    UTexture2D* ImageItem;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Инвентарь", DisplayName="Можно снарядить?")
    bool bCanEquip = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь",
        DisplayName="Можно использовать в крафте?", meta=(EditCondition = "!bCanEquip", EditConditionHides))
    bool bCanCraft = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь",
        DisplayName="Стакается предмет?", meta=(EditCondition = "!bCanEquip", EditConditionHides))
    bool bStack = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь",
        DisplayName = "Можно использовать?", meta=(EditCondition = "!bCanEquip", EditConditionHides))
    bool bCanUse = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Инвентарь",
        DisplayName="Макс. количество",
        meta=(ToolTip = "Максимальное количество предметов в стаке", EditCondition="bStack", EditConditionHides))
    int32 MaxCount = 50;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Инвентарь",
        DisplayName="Настройки эффектов",
        meta=(ToolTip = "Настройки эффектов накладываемые на игрока, которые изменяют значения выбранных атрибутов игрока при использовании предмета.",
            EditCondition="bCanUse", EditConditionHides))
    TArray<FItemEffect> ItemEffect;

    UPROPERTY()
    TArray<float> ItemsDurability;

    UPROPERTY()
    FName InteractRowName;

    UPROPERTY();
    bool bIsChecked = false;

    FInventoryItem()
        : ImageItem(nullptr)
    {
    }

    FInventoryItem(int i)
        : ImageItem(nullptr) { SlotIndex = i; }

    FInventoryItem(const FInventoryItem* OtherItem);

    FInventoryItem operator =(const FInventoryItem& Other)
    {
        this->InteractRowName = Other.InteractRowName;
        this->Name = Other.Name;
        this->Description = Other.Description;
        this->SlotIndex = Other.SlotIndex;
        this->Count = Other.Count;
        this->CostDurability = Other.CostDurability;
        this->TypeComponent = Other.TypeComponent;
        this->ImageItem = Other.ImageItem;
        this->bCanEquip = Other.bCanEquip;
        this->bCanCraft = Other.bCanCraft;
        this->bStack = Other.bStack;
        this->bCanUse = Other.bCanUse;
        this->MaxCount = Other.MaxCount;
        this->ItemEffect = Other.ItemEffect;
        this->ItemCategory = Other.ItemCategory;
        this->ItemsDurability = Other.ItemsDurability;
        this->bIsChecked = Other.bIsChecked;
        return *this;
    }

    bool operator ==(const FInventoryItem& Other) const
    {
        return this->InteractRowName == Other.InteractRowName;
    }

    bool operator !=(const FInventoryItem& Other) const
    {
        return this->InteractRowName != Other.InteractRowName;
    }
};

USTRUCT(BlueprintType)
struct FCraftItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Крафт",
        DisplayName="Предмет")
    FDataTableRowHandle Item;

    UPROPERTY()
    bool bIsChecked = false;

    bool operator ==(const FCraftItem& Other) const
    {
        return this->Item == Other.Item;
    }

    bool operator !=(const FCraftItem& Other) const
    {
        return this->Item != Other.Item;
    }
};

USTRUCT(BlueprintType)
struct FRecipeItem : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Крафт",
        DisplayName="Необходимые ингредиенты", meta=(ToolTip = "Необходимые предметы, который игрок должен будет положить в слоты крафта."))
    TArray<FCraftItem> RequiredIngredients;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Крафт",
        DisplayName="Нужен ли маленький костер?", meta=(ToolTip = "Нужен ли будет находиться игроку рядом с маленьким костром для крафта предмета."))
    bool bIsNeedSmallFire = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Крафт",
        DisplayName="Нужен ли большой костер?", meta=(ToolTip = "Нужен ли будет находиться игроку рядом с большим костром для крафта предмета."))
    bool bIsNeedCampFire = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Крафт",
        DisplayName="Нужен ли верстак?", meta=(ToolTip = "Нужен ли будет находиться игроку рядом с верстаком для крафта предмета."))
    bool bIsNeedWorkbench = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Крафт",
        DisplayName="Выходной предмет", meta=(ToolTip = "Предмет, который будет произведен."))
    FDataTableRowHandle OutputItem;
};

