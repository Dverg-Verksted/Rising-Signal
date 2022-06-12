// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/JournalSystem/BaseEntity/JournalBaseEntity.h"
#include "JournalNoteEntity.generated.h"

/**
 *
 */
UCLASS()
class RISINGSIGNAL_API UJournalNoteEntity : public UJournalBaseEntity
{
	GENERATED_BODY()

public:
	UJournalNoteEntity() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Journal",
		meta = (DisplayName = "Дата: -- день", ToolTip = "Значение для отображения дня"))
	FText Date;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Journal",
		meta = (DisplayName = "Описание", ToolTip = "Описание какого-либо события, действия в широком формате"))
	FText Description;
};
