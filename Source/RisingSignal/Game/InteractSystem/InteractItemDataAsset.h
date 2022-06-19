// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InteractItemDataAsset.generated.h"

UENUM(BlueprintType)
enum class ETypeItem : uint8
{
    MoveItem UMETA(DisplayName = "Обычный предмет"),
    InvItem UMETA(DisplayName = "Инвентарный предмет"),
    NoteItem UMETA(DisplayName = "Записка"),
    AudioItem UMETA(DisplayName = "Аудио"),
    PhotoItem UMETA(DisplayName = "Фото")
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
     * @public Get mesh item
     * @return UStaticMesh*
     **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | GetData")
    FORCEINLINE FSoftObjectPath GetMeshItem() const { return this->MeshItem; }

    /**
      * @public Get type item
      * @return ETypeItem
     **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | GetData")
    FORCEINLINE ETypeItem GetTypeItem() const { return this->TypeItem; }

    /**
      * @public Get Name Item;
      * @return FText
     **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | GetData")
    FORCEINLINE FText GetNameItem() const { return this->NameItem; }

    /**
      * @public Get Description Item
      * @return FText
     **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | GetData")
    FORCEINLINE FText GetDescriptionItem() const { return this->DescriptionItem; }

    /**
      * @public Get Texture Item
      * @return FSoftObjectPath
     **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | InventoryItem | GetData")
    FORCEINLINE FSoftObjectPath GetTextureItem() const { return this->TextureItem; }

    /**
      * @public Is Stackable
      * @return bool
     **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | InventoryItem | GetData")
    FORCEINLINE bool IsStackable() const { return this->bStackable; }

    /**
      * @public Is Can Use
      * @return bool
     **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | InventoryItem | GetData")
    FORCEINLINE bool IsCanUse() const { return this->bCanUse; }

    /**
      * @public Is Can Craft
      * @return bool
     **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | InventoryItem | GetData")
    FORCEINLINE bool IsCanCraft() const { return this->bCanCraft; }

    /**
  * @public Get texture Photo journal
  * @return FSoftObjectPath
 **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | PhotoItem | GetData")
    FORCEINLINE FSoftObjectPath GetTexturePhotoItem() const { return this->JournalPhoto; }

private:
    // @private Type item
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item", meta = (DisplayName = "Тип предмета", ToolTip = "Укажите тип предмета"))
    ETypeItem TypeItem = ETypeItem::MoveItem;
    
    // @private Name item
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Имя предмета", ToolTip = "Укажите имя предмета"))
    FText NameItem = FText();

    // @private Description item
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Описание предмета", MultiLine, ToolTip = "Опишите предмет"))
    FText DescriptionItem = FText();
    
    // @private Static mesh item path
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (AllowedClasses = "StaticMesh", DisplayName = "Мэш предмета", ToolTip = "Укажите статик мэш предмета"))
    FSoftObjectPath MeshItem;

#pragma region InvItem
    
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
        meta = (DisplayName = "Предмет можно использовать?", ToolTip = "Поставьте галочку если данный предмет можно использовать",
        EditCondition = "TypeItem == ETypeItem::InvItem", EditConditionHides))
    bool bCanUse = false;

    // @private State is Stackable
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Предмет можно скрафтить?", ToolTip = "Поставьте галочку если данный предмет можно скрафтить",
        EditCondition = "TypeItem == ETypeItem::InvItem", EditConditionHides))
    bool bCanCraft = false;

#pragma endregion

#pragma region NoteItem



#pragma endregion

#pragma region AudioItem



#pragma endregion

#pragma region PhotoItem

    UPROPERTY(EditAnywhere, Category = "Settings Data Item", meta = (DisplayName = "Фотография", ToolTip = "Здесь хранятся фотографии",
        EditCondition = "TypeItem == ETypeItem::PhotoItem", EditConditionHides, AllowedClasses = "Texture2D"))
    FSoftObjectPath JournalPhoto;

#pragma endregion
    
};
