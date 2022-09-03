#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InteractDataItem.generated.h"


class ARSInteractStaticItemBase;
UENUM(BlueprintType)
enum class ETypeItem : uint8
{
    None,
    StaticItem UMETA(DisplayName = "Статичный предмет"),
    MovableItem UMETA(DisplayName = "Перемещаемый предмет"),
    InvItem UMETA(DisplayName = "Инвентарный предмет"),
    NoteItem UMETA(DisplayName = "Записка"),
    AudioItem UMETA(DisplayName = "Аудио"),
    PhotoItem UMETA(DisplayName = "Фото")
};

USTRUCT(BlueprintType)
struct FDataInteract : public FTableRowBase
{
    GENERATED_BODY()

    // @private Type item
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Тип предмета", ToolTip = "Укажите тип предмета"))
    ETypeItem TypeItem = ETypeItem::StaticItem;

    // @private Name item
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Имя предмета", ToolTip = "Укажите имя предмета"))
    FText Name = FText();

    // @private Description item
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Описание предмета", MultiLine, ToolTip = "Опишите предмет"))
    FText DescriptionItem = FText();

    // @private Static mesh item path
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (
            AllowedClasses = "StaticMesh",
            DisplayName = "Мэш предмета",
            ToolTip = "Укажите статик мэш предмета",
            EditCondition = "TypeItem != ETypeItem::StaticItem",
            EditConditionHides
        )
    )
    FSoftObjectPath MeshItem;

#pragma region StaticItem

    // @private Interval count range
    // UPROPERTY(EditAnywhere, Category = "Settings Data Item",
    //     meta = (
    //         DisplayName = "Предмет",
    //         ToolTip = "Укажите актор предмета",
    //         EditCondition = "TypeItem == ETypeItem::StaticItem",
    //         EditConditionHides
    //     )
    // )
    // TSubclassOf<ARSInteractStaticItemBase> StaticActorClass = nullptr;



#pragma endregion

#pragma region MovItem

#pragma endregion

#pragma region InvItem

    // @private Interval count range
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (
            DisplayName = "Правила предметов",
            ToolTip = "Укажите таблицу и строку данной таблицы правил по передачи их в инвентарь",
            EditCondition = "TypeItem == ETypeItem::InvItem",
            EditConditionHides
        )
    )
    FDataTableRowHandle RowRuleInvItem;

    // @private Interval count range
    // UPROPERTY(EditInstanceOnly, Category = "Settings Data Item",
    //     meta = (
    //         DisplayName = "Количество предметов",
    //         ToolTip = "Укажите количество подбираемых предметов",
    //         EditCondition = "TypeItem == ETypeItem::InvItem",
    //         EditConditionHides
    //     )
    // )
    // int32 ItemCount = 1;

#pragma endregion

#pragma region NoteItem

    // @private Note entity
    FPrimaryAssetType JournalNoteType;

    // @private Note map Description
    UPROPERTY(EditAnywhere, Category="Settings Data Item",
        meta = (AllowedClasses= "World", EditCondition = "TypeItem == ETypeItem::NoteItem",
            EditConditionHides))
    FSoftObjectPath NoteMap;

    // @private Note Header
    UPROPERTY(EditAnywhere, Category = "Settings Data Item",
        meta = (DisplayName = "Заголовок журнала Записки", ToolTip = "Заголовок журнала записки", EditCondition =
            "TypeItem == ETypeItem::NoteItem",
            EditConditionHides))
    FText NoteHeader;

    // @private Note Date
    UPROPERTY(EditAnywhere, Category = "Settings Data Item",
        meta = (DisplayName = "Дата: -- день", ToolTip = "Отображение дня, целое число",
            EditCondition = "TypeItem == ETypeItem::NoteItem", EditConditionHides))
    FText NoteDate;

    // @private Note Description
    UPROPERTY(EditAnywhere, Category = "Settings Data Item",
        meta = (DisplayName = "Описание", ToolTip = "Описание чего-либо в широком смысле",
            EditCondition = "TypeItem == ETypeItem::NoteItem", EditConditionHides))
    FText NoteDescription;

#pragma endregion

#pragma region AudioItem

    // @private Audio entity
    FPrimaryAssetType JournalAudioType;

    UPROPERTY(EditAnywhere, Category="Settings Data Item",
        meta = (AllowedClasses= "World", EditCondition = "TypeItem == ETypeItem::AudioItem", EditConditionHides))
    FSoftObjectPath AudioMap;

    // @private Audio Header
    UPROPERTY(EditAnywhere, Category = "Settings Data Item",
        meta = (DisplayName = "Заголовок журнала аудиозаписи", ToolTip = "Заголовок журнала аудиозаписи", EditCondition =
            "TypeItem == ETypeItem::AudioItem", EditConditionHides))
    FText AudioHeader;

    UPROPERTY(EditAnywhere, Category = "Settings Data Item",
        meta = (DisplayName = "Аудиозапись", ToolTip = "Добавление аудиозаписи типа USoundCue",
            EditCondition = "TypeItem == ETypeItem::AudioItem", EditConditionHides, AllowedClasses = "SoundCue"))
    FSoftObjectPath JournalAudio;

#pragma endregion

#pragma region PhotoItem

    // @private Photo entity
    FPrimaryAssetType JournalPhotoType;

    UPROPERTY(EditAnywhere, Category="Settings Data Item",
        meta = (AllowedClasses= "World", EditCondition = "TypeItem == ETypeItem::PhotoItem", EditConditionHides))
    FSoftObjectPath PhotoMap;

    // @private Photo Header
    UPROPERTY(EditAnywhere, Category = "Settings Data Item",
        meta = (DisplayName = "Заголовок журнала фотографии", ToolTip = "Заголовок журнала фотографии", EditCondition =
            "TypeItem == ETypeItem::PhotoItem", EditConditionHides))
    FText PhotoHeader;

    UPROPERTY(EditAnywhere, Category = "Settings Data Item",
        meta = (DisplayName = "Фотография", ToolTip = "Добавление фотографии типа Texture2D",
            EditCondition = "TypeItem == ETypeItem::PhotoItem", EditConditionHides, AllowedClasses = "Texture2D"))
    FSoftObjectPath JournalPhoto;

#pragma endregion
};
