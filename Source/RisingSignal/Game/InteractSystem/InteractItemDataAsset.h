// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InteractItemDataAsset.generated.h"

UENUM(BlueprintType)
enum class ETypeItem : uint8
{
    MoveItem UMETA(DisplayName = "Обычный предмет"),
    InvItem UMETA(DisplayName = "Инвентарный предмет")
};

/**
 * @class Data set for the items
 */
UCLASS()
class RISINGSIGNAL_API UInteractItemDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    /**
     * @public Get mesh item;
     **/
    FORCEINLINE const FSoftObjectPath& GetMeshItem() const { return this->MeshItem; }

private:
    // @private Type item
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item", meta = (DisplayName = "Тип предмета", ToolTip = "Укажите тип предмета"))
    ETypeItem TypeItem = ETypeItem::MoveItem;

    // @private Static mesh item path
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (AllowedClasses = "StaticMesh", DisplayName = "Мэш предмета", ToolTip = "Укажите статик мэш предмета"))
    FSoftObjectPath MeshItem;

    // @private Name item
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Имя предмета", ToolTip = "Укажите имя предмета", EditCondition = "TypeItem == ETypeItem::InvItem",
            EditConditionHides))
    FText NameItem = FText();

    // @private Description item
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Описание предмета", MultiLine, ToolTip = "Опишите предмет", EditCondition = "TypeItem == ETypeItem::InvItem",
            EditConditionHides))
    FText DescriptionItem = FText();

    // @private Description item
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Иконка предмета", AllowedClasses = "Texture2D", ToolTip = "Выберите иконку предмета",
            EditCondition = "TypeItem == ETypeItem::InvItem", EditConditionHides))
    FSoftObjectPath TextureItem;

    // @private State is Stackable
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Предмет стаковый?", ToolTip = "Данный предмет стаковый ?", EditCondition = "TypeItem == ETypeItem::InvItem",
            EditConditionHides))
    bool bStackable = false;

    // @private State is Stackable
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Максимальное количество", ToolTip = "Укажите максимальное количество стака",
            EditCondition = "TypeItem == ETypeItem::InvItem && bStackable == true", EditConditionHides, ClampMin = "2", ClampMax = "2000"))
    int32 CountMaxStack = 2;
};
