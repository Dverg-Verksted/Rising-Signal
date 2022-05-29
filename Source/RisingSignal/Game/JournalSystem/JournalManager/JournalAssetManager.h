// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Game/JournalSystem/BaseEntity/JournalBaseEntity.h"
#include "JournalAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API UJournalAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UJournalAssetManager(){}

	static const FPrimaryAssetType JournalNoteEntity;
	static const FPrimaryAssetType JournalAudioEntity;
	static const FPrimaryAssetType JournalPhotoEntity;

	static UJournalAssetManager &Get();

	UJournalBaseEntity *ForceLoadJournal(const FPrimaryAssetId &PrimaryAssetId, bool bLogWarning = true);
};
