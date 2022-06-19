// It is owned by the company Dverg Verksted.

#include "Game/JournalSystem/JournalSystem.h"
#include "IDetailTreeNode.h"

UJournalSystem::UJournalSystem()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UJournalSystem::AddNoteItem(UJournalNoteEntity* NewNoteObj)
{
    if (!NewNoteObj)
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The Note object: %s is nullptr"), *GetName()));
        return;
    }
    if (NewNoteObj->Date.IsEmpty())
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The field Date: %s is empty"), *GetName()));
        return;
    }
    if (NewNoteObj->Description.IsEmpty())
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The field Description: %s is empty"), *GetName()));
        return;
    }
    
    const FString AssetName = NewNoteObj->GetChapterName();
    const int32 IndexStruct = GetNoteChapterIndex(AssetName);
    this->ArrNoteObj[IndexStruct].ArrNote.AddUnique(NewNoteObj);
}

void UJournalSystem::AddAudioItem(UJournalAudioEntity* NewAudioObj)
{
    if (!NewAudioObj)
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The Audio object: %s is nullptr"), *GetName()));;
        return;
    }
    if (NewAudioObj->JournalAudio.IsNull())
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The field JournalAudio: %s is not pointing to a live object"), *GetName()));
        return;
    }

    const FString AssetName = NewAudioObj->GetChapterName();
    const int32 IndexStruct = GetAudioChapterIndex(AssetName);
    this->ArrAudioObj[IndexStruct].ArrAudio.AddUnique(NewAudioObj);
}

void UJournalSystem::AddPhotoItem(UJournalPhotoEntity* NewPhotoObj)
{
    if (!NewPhotoObj)
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The Photo object: %s is nullptr"), *GetName()));;
        return;
    }
    if (NewPhotoObj->JournalPhoto.IsNull())
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The field JournalPhoto: %s is not pointing to a live object"), *GetName()));
        return;
    }

    const FString AssetName = NewPhotoObj->GetChapterName();
    const int32 IndexStruct = GetAudioChapterIndex(AssetName);
    this->ArrPhotoObj[IndexStruct].ArrPhoto.AddUnique(NewPhotoObj);
}

void UJournalSystem::GetNextJournalState(EStateJournalSystem NextState)
{
    StateJournalSystem = NextState;
}

UJournalNoteEntity* UJournalSystem::GetNoteObjByIndex(int32 LevelIndex, int32 NotesIndex)
{
    if (this->ArrNoteObj.IsValidIndex(LevelIndex))
    {
        if (this->ArrNoteObj[LevelIndex].ArrNote.IsValidIndex(NotesIndex))
        {
            return this->ArrNoteObj[LevelIndex].ArrNote[NotesIndex];
        }
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Index: %d is not valid"), NotesIndex));
        
        return nullptr;
    }
    LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Index: %d is not valid"), LevelIndex));

    return nullptr;
}

UJournalAudioEntity* UJournalSystem::GetAudioObjByIndex(int32 LevelIndex, int32 AudioIndex)
{
    if (this->ArrAudioObj.IsValidIndex(LevelIndex))
    {
        if (this->ArrAudioObj[LevelIndex].ArrAudio.IsValidIndex(AudioIndex))
        {
            return this->ArrAudioObj[LevelIndex].ArrAudio[AudioIndex];
        }
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Index: %d is not valid"), AudioIndex));

        return nullptr;
    }
    LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Index: %d is not valid"), LevelIndex));

    return nullptr;
}

UJournalPhotoEntity* UJournalSystem::GetPhotoObjByIndex(int32 LevelIndex, int32 PhotoIndex)
{
    if (this->ArrPhotoObj.IsValidIndex(LevelIndex))
    {
        if (this->ArrPhotoObj[LevelIndex].ArrPhoto.IsValidIndex(PhotoIndex))
        {
            return this->ArrPhotoObj[LevelIndex].ArrPhoto[PhotoIndex];
        }
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Index: %d is not valid"), PhotoIndex));

        return nullptr;
    }
    LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Index: %d is not valid"), LevelIndex));

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

TArray<FChapterDataNote> UJournalSystem::GetAllNote()
{
    // TODO подумать и добавить проверки?
    return (this->ArrNoteObj);
}

TArray<FChapterDataAudio> UJournalSystem::GetAllAudio()
{
    // TODO подумать и добавить проверки?
    return (this->ArrAudioObj);
}

TArray<FChapterDataPhoto> UJournalSystem::GetAllPhoto()
{
    // TODO подумать и добавить проверки?
    return (this->ArrPhotoObj);
}

int32 UJournalSystem::GetNoteChapterIndex(FString NoteName)
{
    if (NoteName.IsEmpty())
    {
        return 0;
    }
    
    int32 Index = 0;

    for (auto arr : this->ArrNoteObj)
    {
        if (this->ArrNoteObj[Index].ChapterName == NoteName)
        {
            return Index;
        }
        Index++;
    }

    return 0;
}

int32 UJournalSystem::GetAudioChapterIndex(FString AudioName)
{
    if (AudioName.IsEmpty())
    {
        return 0;
    }
    
    int32 Index = 0;
    
    for (auto arr : this->ArrAudioObj)
    {
        if (this->ArrAudioObj[Index].ChapterName == AudioName)
        {
            return Index;
        }
        Index++;
    }
    
    return 0;

}

int32 UJournalSystem::GetPhotoChapterIndex(FString PhotoName)
{
    if (PhotoName.IsEmpty())
    {
        return 0;
    }
    
    int32 Index = 0;
    
    for (auto arr : this->ArrPhotoObj)
    {
       
        if (this->ArrPhotoObj[Index].ChapterName == PhotoName)
        {
            return Index;
        }
        Index++;
    }
    
    return 0;
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
