// It is owned by the company Dverg Verksted.


#include "Game/JournalSystem/JournalManager/JournalAssetManager.h"

const FPrimaryAssetType UJournalAssetManager::JournalNoteEntity = TEXT("Записка");
const FPrimaryAssetType UJournalAssetManager::JournalAudioEntity = TEXT("Звуковая дорожка");
const FPrimaryAssetType UJournalAssetManager::JournalPhotoEntity = TEXT("Фотография");

UJournalAssetManager &UJournalAssetManager::Get()
{
	UJournalAssetManager *This = Cast<UJournalAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Неверный AssetManager в DefaultEngine.ini, должен быть JournalAssetManager!"));
		return *NewObject<UJournalAssetManager>();
	}
}

UJournalBaseEntity* UJournalAssetManager::ForceLoadJournal(const FPrimaryAssetId &PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath((PrimaryAssetId));
	UJournalBaseEntity *LoadedItem = Cast<UJournalBaseEntity>(ItemPath.TryLoad());

	if(bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp,Warning, TEXT("Ошибка загрузки сущности для индентификатора: %s!"), *PrimaryAssetId.ToString());
	}
	
	return LoadedItem;
}