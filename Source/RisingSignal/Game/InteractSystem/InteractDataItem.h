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
        meta = (DisplayName = "Имя предмета", ToolTip = "Укажите имя предмета", EditCondition = "TypeItem != ETypeItem::InvItem",
            EditConditionHides))
    FText Name = FText();

    // @private Description item
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (DisplayName = "Описание предмета", MultiLine, ToolTip = "Опишите предмет", EditCondition = "TypeItem != ETypeItem::InvItem",
            EditConditionHides))
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

    // @private InteractText
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (
            DisplayName = "Использовать уникальный текст взаимодействия",
            ToolTip = "Если включено, текст будет браться из соответствующего поля. Если выключено - будет отображаться имя предмета"
        )
    )
    bool bCustomInteractText = false;

    // @private InteractText
    UPROPERTY(EditDefaultsOnly, Category = "Settings Data Item",
        meta = (
            DisplayName = "Текст взаимодействия",
            ToolTip = "Введите текст, который будет отображаться при взаимодействии",
            EditCondition = "bCustomInteractText",
            EditConditionHides
        )
    )
    FText InteractText;

    // @private Attached Map Description
    UPROPERTY(EditDefaultsOnly, Category="Settings Data Item", DisplayName = " Привязанная карта",
        meta = (
            ToolTip = "Укажите карту, принадлежность к которой будет указываться в журнале",
            AllowedClasses= "World",
            EditCondition = "TypeItem == ETypeItem::NoteItem || TypeItem == ETypeItem::AudioItem || TypeItem == ETypeItem::PhotoItem",
            EditConditionHides
        ))
    FSoftObjectPath AttachedMap;


#pragma region StaticItem

    // @private StaticActorClass
    UPROPERTY(EditAnywhere, Category = "Settings Data Item",
        meta = (
            DisplayName = "Актор предмета",
            ToolTip = "Укажите актор предмета",
            EditCondition = "TypeItem == ETypeItem::StaticItem",
            EditConditionHides
        )
    )
    TSoftClassPtr<ARSInteractStaticItemBase> StaticActorClassPtr = nullptr;


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

#pragma endregion

#pragma region NoteItem

    // @private Note entity
    FPrimaryAssetType JournalNoteType;

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
