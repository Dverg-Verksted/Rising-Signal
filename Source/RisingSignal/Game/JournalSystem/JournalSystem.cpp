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
    
    this->ArrNoteObj.AddUnique(NewNoteObj);
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
        LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("The field JournalAudio: %s is not pointing to a live object"), *NewAudioObj.AudioMap.GetAssetName()));
        return;
    }
    
    this->ArrAudioObj.AddUnique(NewAudioObj);
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
        LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("The field JournalPhoto: %s is not pointing to a live object"), *NewPhotoObj.PhotoMap.GetAssetName()));
        return;
    }

    this->ArrPhotoObj.AddUnique(NewPhotoObj);
}

void UJournalSystem::GetNextJournalState(EStateJournalSystem NextState)
{
    StateJournalSystem = NextState;
}

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
