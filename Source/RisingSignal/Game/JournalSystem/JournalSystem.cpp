// It is owned by the company Dverg Verksted.

#include "Game/JournalSystem/JournalSystem.h"

#include "IDetailTreeNode.h"

UJournalSystem::UJournalSystem()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UJournalSystem::AddNoteItem(UJournalNoteEntity* NewNoteObj)
{
    if (!NewNoteObj) return;                                                      // TODO добавить логирование
    if (NewNoteObj->Date.IsEmpty() || NewNoteObj->Description.IsEmpty()) return;  // TODO добавить логирование

    this->ArrNoteObj.Add(NewNoteObj);
}

void UJournalSystem::AddAudioItem(UJournalAudioEntity* NewAudioObj)
{
    if (!NewAudioObj) return;                        // TODO добавить логирование
    if (NewAudioObj->JournalAudio.IsNull()) return;  // TODO добавить логирование

    this->ArrAudioObj.Add(NewAudioObj);
}

void UJournalSystem::AddPhotoItem(UJournalPhotoEntity* NewPhotoObj)
{
    if (!NewPhotoObj) return;                        // TODO добавить логирование
    if (NewPhotoObj->JournalPhoto.IsNull()) return;  // TODO добавить логирование

    this->ArrPhotoObj.Add(NewPhotoObj);
}

void UJournalSystem::GetNextJournalState(EStateJournalSystem NextState)
{
    StateJournalSystem = NextState;
}

UJournalNoteEntity* UJournalSystem::GetNoteObjByIndex(int32 Index)
{
    if (this->ArrNoteObj.IsValidIndex(Index))
    {
        return this->ArrNoteObj[Index];  // TODO добавить логирование
    }
    return nullptr;
}

UJournalAudioEntity* UJournalSystem::GetAudioObjByIndex(int32 Index)
{
    if (this->ArrAudioObj.IsValidIndex(Index))
    {
        return this->ArrAudioObj[Index];  // TODO добавить логирование
    }
    return nullptr;
}

UJournalPhotoEntity* UJournalSystem::GetPhotoObjByIndex(int32 Index)
{
    if (this->ArrPhotoObj.IsValidIndex(Index))
    {
        return this->ArrPhotoObj[Index];  // TODO добавить логирование
    }
    return nullptr;
}

int32 UJournalSystem::GetArrNoteObjSize()
{
    // TODO подумать и добавить проверки?
    return ArrNoteObj.Num();
}

int32 UJournalSystem::GetArrAudioObjSize()
{
    // TODO подумать и добавить проверки?
    return ArrAudioObj.Num();
}

int32 UJournalSystem::GetArrPhotoObjSize()
{
    // TODO подумать и добавить проверки?
    return ArrPhotoObj.Num();
}

void UJournalSystem::DeleteNoteObjByIndex(int32 Index)
{
    if (this->ArrNoteObj.IsValidIndex(Index))
    {
        this->ArrNoteObj.RemoveAt(Index);  // TODO добавить логирование
    }
    // TODO добавить логирование
}

void UJournalSystem::DeleteAudioObjByIndex(int32 Index)
{
    if (this->ArrAudioObj.IsValidIndex(Index))
    {
        this->ArrAudioObj.RemoveAt(Index);  // TODO добавить логирование
    }
    // TODO добавить логирование
}

void UJournalSystem::DeletePhotoObjByIndex(int32 Index)
{
    if (this->ArrPhotoObj.IsValidIndex(Index))
    {
        this->ArrPhotoObj.RemoveAt(Index);  // TODO добавить логирование
    }
    // TODO добавить логирование
}

void UJournalSystem::BeginPlay()
{
    Super::BeginPlay();
}
