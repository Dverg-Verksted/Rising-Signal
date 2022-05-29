// It is owned by the company Dverg Verksted.


#include "Game/JournalSystem/BaseEntity/JournalBaseEntity.h"

FString UJournalBaseEntity::GetIdentifierString()
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UJournalBaseEntity::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(JournalType, GetFName());
}