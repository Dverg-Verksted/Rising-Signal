// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/JournalSystem/BaseEntity/JournalBaseEntity.h"
#include "Game/JournalSystem/JournalManager/JournalAssetManager.h"
#include "Sound/SoundCue.h"
#include "JournalAudioEntity.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API UJournalAudioEntity : public UJournalBaseEntity
{
	GENERATED_BODY()
public:
	UJournalAudioEntity()
	{
		JournalType = UJournalAssetManager::JournalAudioEntity;
	}
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Journal", meta=(DisplayName="Аудио-объект журнала", ToolTip="Здесь хранятся аудиозаписи"))
	TSoftObjectPtr<USoundCue> JournalAudio;
};
