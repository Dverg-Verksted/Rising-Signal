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
      * @public Get type Note journal
      * @return FPrimaryAssetType
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | NoteItem | GetData")
    FORCEINLINE FPrimaryAssetType GetTypeNoteItem() const { return this->JournalNoteType; }

    /**
      * @public Get type Note journal
      * @return FPrimaryAssetType
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | NoteItem | GetData")
    FORCEINLINE FSoftObjectPath GetNoteMap() const { return this->NoteMap; }

    /**
    * @public Get type Note journal
    * @return FPrimaryAssetType
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | NoteItem | GetData")
    FORCEINLINE FText GetNoteHeader() const { return this->NoteHeader; }

    /**
    * @public Get type Note journal
    * @return FPrimaryAssetType
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | NoteItem | GetData")
    FORCEINLINE FText GetNoteDate() const { return this->NoteDate; }

    /**
    * @public Get type Note journal
    * @return FPrimaryAssetType
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | NoteItem | GetData")
    FORCEINLINE FText GetNoteDescription() const { return this->NoteDescription; }

    /**
      * @public Get type Audio journal
      * @return FPrimaryAssetType
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | AudioItem | GetData")
    FORCEINLINE FPrimaryAssetType GetTypeAudioItem() const { return this->JournalAudioType; }

    /**
      * @public Get texture Audio journal
      * @return FSoftObjectPath
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | AudioItem | GetData")
    FORCEINLINE FSoftObjectPath GetTextureAudioItem() const { return this->JournalAudio; }

    /**
    * @public Get type Note journal
    * @return FPrimaryAssetType
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | AudioItem | GetData")
    FORCEINLINE FSoftObjectPath GetAudioMap() const { return this->AudioMap; }

    /**
    * @public Get type Note journal
    * @return FPrimaryAssetType
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | AudioItem | GetData")
    FORCEINLINE FText GetAudioHeader() const { return this->AudioHeader; }
    
    /**
      * @public Get type Audio journal
      * @return FPrimaryAssetType
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | PhotoItem | GetData")
    FORCEINLINE FPrimaryAssetType GetTypePhotoItem() const { return this->JournalPhotoType; }
    
    /**
      * @public Get texture Photo journal
      * @return FSoftObjectPath
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | PhotoItem | GetData")
    FORCEINLINE FSoftObjectPath GetTexturePhotoItem() const { return this->JournalPhoto; }

    /**
    * @public Get type Note journal
    * @return FPrimaryAssetType
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | NoteItem | GetData")
    FORCEINLINE FSoftObjectPath GetPhotoMap() const { return this->PhotoMap; }

    /**
    * @public Get type Note journal
    * @return FPrimaryAssetType
    **/
    UFUNCTION(BlueprintCallable, Category = "InteractItemDataAsset | NoteItem | GetData")
    FORCEINLINE FText GetPhotoHeader() const { return this->PhotoHeader; }

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
    
    // @private Note entity
    FPrimaryAssetType JournalNoteType;

    // @private Note map Description
    UPROPERTY(EditAnywhere, Category="Settings Data Item", meta = (AllowedClasses= "World", EditCondition = "TypeItem == ETypeItem::NoteItem",
        EditConditionHides))
    FSoftObjectPath NoteMap;

    // @private Note Header
    UPROPERTY(EditAnywhere, Category = "Settings Data Item",
    meta = (DisplayName = "Заголовок журнала Записки", ToolTip = "Заголовок журнала записки", EditCondition = "TypeItem == ETypeItem::NoteItem",
        EditConditionHides))
    FText NoteHeader;

    // @private Note Date
    UPROPERTY(EditAnywhere, Category = "Settings Data Item", meta = (DisplayName = "Дата: -- день", ToolTip = "Отображение дня, целое число",
        EditCondition = "TypeItem == ETypeItem::NoteItem", EditConditionHides))
    FText NoteDate;

    // @private Note Description
    UPROPERTY(EditAnywhere, Category = "Settings Data Item", meta = (DisplayName = "Описание", ToolTip = "Описание чего-либо в широком смысле",
        EditCondition = "TypeItem == ETypeItem::NoteItem", EditConditionHides))
    FText NoteDescription;

#pragma endregion

#pragma region AudioItem
    
    // @private Audio entity
    FPrimaryAssetType JournalAudioType;

    UPROPERTY(EditAnywhere, Category="Settings Data Item", meta = (AllowedClasses= "World", EditCondition = "TypeItem == ETypeItem::AudioItem", EditConditionHides))
    FSoftObjectPath AudioMap;

    // @private Audio Header
    UPROPERTY(EditAnywhere, Category = "Settings Data Item",
        meta = (DisplayName = "Заголовок журнала аудиозаписи", ToolTip = "Заголовок журнала аудиозаписи", EditCondition = "TypeItem == ETypeItem::AudioItem", EditConditionHides))
    FText AudioHeader;
    
    UPROPERTY(EditAnywhere, Category = "Settings Data Item", meta = (DisplayName = "Аудиозапись", ToolTip = "Добавление аудиозаписи типа USoundCue",
        EditCondition = "TypeItem == ETypeItem::AudioItem", EditConditionHides, AllowedClasses = "SoundCue"))
    FSoftObjectPath JournalAudio;

#pragma endregion

#pragma region PhotoItem
    
    // @private Photo entity
    FPrimaryAssetType JournalPhotoType;

    UPROPERTY(EditAnywhere, Category="Settings Data Item", meta = (AllowedClasses= "World", EditCondition = "TypeItem == ETypeItem::PhotoItem", EditConditionHides))
    FSoftObjectPath PhotoMap;

    // @private Photo Header
    UPROPERTY(EditAnywhere, Category = "Settings Data Item",
        meta = (DisplayName = "Заголовок журнала фотографии", ToolTip = "Заголовок журнала фотографии", EditCondition = "TypeItem == ETypeItem::PhotoItem", EditConditionHides))
    FText PhotoHeader;
    
    UPROPERTY(EditAnywhere, Category = "Settings Data Item", meta = (DisplayName = "Фотография", ToolTip = "Добавление фотографии типа Texture2D",
        EditCondition = "TypeItem == ETypeItem::PhotoItem", EditConditionHides, AllowedClasses = "Texture2D"))
    FSoftObjectPath JournalPhoto;

#pragma endregion
    
};
