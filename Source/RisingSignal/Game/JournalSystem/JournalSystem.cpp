// It is owned by the company Dverg Verksted.

#include "Game/JournalSystem/JournalSystem.h"
#include "Library/RSFunctionLibrary.h"

UJournalSystem::UJournalSystem()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UJournalSystem::AddNoteItem(FInteractItemNote& NewNoteObj)
{
    if (NewNoteObj.NoteDate.IsEmpty())
    {
        LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("The field Date: %s is empty"), *NewNoteObj.NoteDate.ToString()));
        return;
    }
    if (NewNoteObj.NoteHeader.IsEmpty())
    {
        LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("The field Description: %s is empty"), *NewNoteObj.NoteHeader.ToString()));
        return;
    }
    if (NewNoteObj.NoteDescription.IsEmpty())
    {
        LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("The field Description: %s is empty"), *NewNoteObj.NoteDescription.ToString()));
        return;
    }
    
    // const int32 IndexStruct = GetNoteChapterIndex(NewNoteObj.NoteMap.GetAssetName());

    // if (this->ArrNoteObj.IsValidIndex(IndexStruct))
    // {
    //     OnJournalSystemUpdate.Broadcast();
    //     this->ArrNoteObj[IndexStruct].ArrNote.AddUnique(NewNoteObj);
    // }

    this->ArrNoteObj.AddUnique(NewNoteObj);
    
    //debug
    {
    // LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("The field Name: %s "), *GetName()));
    // LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("The field AssetName: %s "), *AssetName));
    // LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("The field IndexStruct: %d "), IndexStruct));
    //
    // for (auto Name : this->ArrNoteObj)
    // {
    //     LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("The field Name: %s "), *GetName()));
    //     LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The field ChapterName: %s "), *Name.ChapterName));
    //     for (auto fields : Name.ArrNote)
    //     {
    //         LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The field ChapterName: %s "), *fields->Date.ToString()));
    //         LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The field ChapterName: %s "), *fields->Description.ToString()));
    //     }
    // }
    }
}

void UJournalSystem::AddAudioItem(FInteractItemAudio& NewAudioObj)
{
    if (NewAudioObj.AudioHeader.IsEmpty())
    {
        LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("The field Description: %s is empty"), *NewAudioObj.AudioHeader.ToString()));
        return;
    }
    if (NewAudioObj.AudioMap.IsNull())
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The field JournalAudio: %s is not pointing to a live object"), *NewAudioObj.AudioMap.GetAssetName()));
        return;
    }

    this->ArrAudioObj.AddUnique(NewAudioObj);
    // const int32 IndexStruct = GetAudioChapterIndex(NewAudioObj.AudioMap.GetAssetName());
    //
    // if (this->ArrAudioObj.IsValidIndex(IndexStruct))
    // {
    //     OnJournalSystemUpdate.Broadcast();
    //     this->ArrAudioObj[IndexStruct].ArrAudio.AddUnique(NewAudioObj);
    // }
    
}

void UJournalSystem::AddPhotoItem(FInteractItemPhoto& NewPhotoObj)
{
    if (NewPhotoObj.PhotoHeader.IsEmpty())
    {
        LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("The field Description: %s is empty"), *NewPhotoObj.PhotoHeader.ToString()));
        return;
    }
    if (NewPhotoObj.PhotoMap.IsNull())
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("The field JournalPhoto: %s is not pointing to a live object"), *NewPhotoObj.PhotoMap.GetAssetName()));
        return;
    }

    this->ArrPhotoObj.AddUnique(NewPhotoObj);
    // const int32 IndexStruct = GetAudioChapterIndex(NewPhotoObj.PhotoMap.GetAssetName());
    //
    // if (this->ArrPhotoObj.IsValidIndex(IndexStruct))
    // {
    //     OnJournalSystemUpdate.Broadcast();
    //     this->ArrPhotoObj[IndexStruct].ArrPhoto.AddUnique(NewPhotoObj);
    // }
    
}

void UJournalSystem::GetNextJournalState(EStateJournalSystem NextState)
{
    StateJournalSystem = NextState;
}

// FInteractItemNote UJournalSystem::GetNoteObjByIndex(int32 LevelIndex, int32 NotesIndex)
// {
//     if (this->ArrNoteObj.IsValidIndex(LevelIndex))
//     {
//         if (this->ArrNoteObj[LevelIndex].ArrNote.IsValidIndex(NotesIndex))
//         {
//             return this->ArrNoteObj[LevelIndex].ArrNote[NotesIndex];
//         }
//         LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Index: %d is not valid"), NotesIndex));
//         return this->ArrNoteObj[LevelIndex].ArrNote[NotesIndex];
//     }
//     LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Index: %d is not valid"), LevelIndex));
//     return this->ArrNoteObj[LevelIndex].ArrNote[NotesIndex];
// }

// FInteractItemAudio UJournalSystem::GetAudioObjByIndex(int32 LevelIndex, int32 AudioIndex)
// {
//     if (this->ArrAudioObj.IsValidIndex(LevelIndex))
//     {
//         if (this->ArrAudioObj[LevelIndex].ArrAudio.IsValidIndex(AudioIndex))
//         {
//             return this->ArrAudioObj[LevelIndex].ArrAudio[AudioIndex];
//         }
//         LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("Index: %d is not valid"), AudioIndex));
//         return this->ArrAudioObj[LevelIndex].ArrAudio[AudioIndex];
//     }
//     LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("Index: %d is not valid"), LevelIndex));
//     return this->ArrAudioObj[LevelIndex].ArrAudio[AudioIndex];
// }
//
// FInteractItemPhoto UJournalSystem::GetPhotoObjByIndex(int32 LevelIndex, int32 PhotoIndex)
// {
//     if (this->ArrPhotoObj.IsValidIndex(LevelIndex))
//     {
//         if (this->ArrPhotoObj[LevelIndex].ArrPhoto.IsValidIndex(PhotoIndex))
//         {
//             return this->ArrPhotoObj[LevelIndex].ArrPhoto[PhotoIndex];
//         }
//         LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Index: %d is not valid"), PhotoIndex));
//         return this->ArrPhotoObj[LevelIndex].ArrPhoto[PhotoIndex];
//     }
//     LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Index: %d is not valid"), LevelIndex));
//     return this->ArrPhotoObj[LevelIndex].ArrPhoto[PhotoIndex];
// }

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

TArray<FInteractItemNote> UJournalSystem::GetAllNote()
{
    // TODO подумать и добавить проверки?
    return (this->ArrNoteObj);
}

TArray<FInteractItemAudio> UJournalSystem::GetAllAudio()
{
    // TODO подумать и добавить проверки?
    return (this->ArrAudioObj);
}

TArray<FInteractItemPhoto> UJournalSystem::GetAllPhoto()
{
    // TODO подумать и добавить проверки?
    return (this->ArrPhotoObj);
}

// int32 UJournalSystem::GetNoteChapterIndex(FString NoteName)
// {
//     if (NoteName.IsEmpty()) return 0;
//     int32 Index = 0;
//
//     for (auto& Arr : this->ArrNoteObj)
//     {
//         
//         if (Arr.ChapterName == NoteName)
//         {
//             return Index;
//         }
//         Index++;
//     }
//
//     return Index;
// }

// int32 UJournalSystem::GetAudioChapterIndex(FString AudioName)
// {
//     if (AudioName.IsEmpty()) return 0;
//     int32 Index = 0;
//     
//     for (auto& Arr : this->ArrAudioObj)
//     {
//         if (Arr.ChapterName == AudioName)
//         {
//             return Index;
//         }
//         Index++;
//     }
//     return Index;
// }
//
// int32 UJournalSystem::GetPhotoChapterIndex(FString PhotoName)
// {
//     if (PhotoName.IsEmpty()) return 0;    
//     int32 Index = 0;
//     
//     for (auto& Arr : this->ArrPhotoObj)
//     {
//         if (Arr.ChapterName == PhotoName)
//         {
//             return Index;
//         }
//         Index++;
//     }
//     return Index;
// }

void UJournalSystem::DeleteNoteObjByIndex(int32 Index)
{
    if (this->ArrNoteObj.IsValidIndex(Index))
    {
        this->ArrNoteObj.RemoveAt(Index);
    }
    LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("The field Index: %d "), Index));
}

void UJournalSystem::DeleteAudioObjByIndex(int32 Index)
{
    if (this->ArrAudioObj.IsValidIndex(Index))
    {
        this->ArrAudioObj.RemoveAt(Index);
    }
    LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("The field Index: %d "), Index));
}

void UJournalSystem::DeletePhotoObjByIndex(int32 Index)
{
    if (this->ArrPhotoObj.IsValidIndex(Index))
    {
        this->ArrPhotoObj.RemoveAt(Index);
    }
    LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("The field Index: %d "), Index));
}

void UJournalSystem::BeginPlay()
{
    Super::BeginPlay();
}
