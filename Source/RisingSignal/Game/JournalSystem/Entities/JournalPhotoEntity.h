// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/JournalSystem/BaseEntity/JournalBaseEntity.h"
#include "JournalPhotoEntity.generated.h"

/**
 *
 */
UCLASS()
class RISINGSIGNAL_API UJournalPhotoEntity : public UJournalBaseEntity
{
    GENERATED_BODY()

public:
    UJournalPhotoEntity() {}

    UPROPERTY(
        EditAnywhere, BlueprintReadOnly, Category = "Journal", meta = (DisplayName = "Фотография", ToolTip = "Здесь хранятся фотографии"))
    TSoftObjectPtr<UTexture2D> JournalPhoto;
};
