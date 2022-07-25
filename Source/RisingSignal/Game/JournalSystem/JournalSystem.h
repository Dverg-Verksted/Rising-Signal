// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JournalSystem.generated.h"

UENUM(BlueprintType)
enum class EStateJournalSystem : uint8
{
    Note,
    Photo,
    Audio
};

USTRUCT(BlueprintType)
struct FInteractItemNote
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал")
    FText NoteHeader;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал")
    FText NoteDescription;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал")
    FText NoteDate;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал", meta = (AllowedClasses= "World"))
    FSoftObjectPath NoteMap;
    
    bool operator==(const FInteractItemNote& Other) const
    {
        return this->NoteHeader.ToString() == Other.NoteHeader.ToString();
    }
};

USTRUCT(BlueprintType)
struct FInteractItemAudio
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал")
    FText AudioHeader;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал", meta = (AllowedClasses= "World"))
    FSoftObjectPath AudioMap;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал", meta = (AllowedClasses = "SoundCue"))
    FSoftObjectPath JournalAudio;

    bool operator==(const FInteractItemAudio& Other) const
    {
        return this->AudioHeader.ToString() == Other.AudioHeader.ToString();
    }
};

USTRUCT(BlueprintType)
struct FInteractItemPhoto
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал")
    FText PhotoHeader;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал", meta = (AllowedClasses= "World"))
    FSoftObjectPath PhotoMap;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал", meta = (AllowedClasses = "Texture2D"))
    FSoftObjectPath JournalPhoto;

    bool operator==(const FInteractItemPhoto& Other) const
    {
        return this->PhotoHeader.ToString() == Other.PhotoHeader.ToString();
    }
};

USTRUCT(BlueprintType)
struct FChapterDataNote
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал")
    FString ChapterName = TEXT("Map_TESTREF");

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал")
    TArray<FInteractItemNote> ArrNote;
};

USTRUCT(BlueprintType)
struct FChapterDataAudio
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал")
    FString ChapterName = TEXT("Map_TESTREF");

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал")
    TArray<FInteractItemAudio> ArrAudio;
};

USTRUCT(BlueprintType)
struct FChapterDataPhoto
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал")
    FString ChapterName = TEXT("Map_TESTREF");

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Журнал")
    TArray<FInteractItemPhoto> ArrPhoto;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJournalSystemUpdateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJournalSystemUpdateSignatureNote,EStateJournalSystem, StateEvent, FChapterDataNote, ItemDataNote);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJournalSystemUpdateSignaturePhoto,EStateJournalSystem, StateEvent, FChapterDataAudio, ItemDataAudio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJournalSystemUpdateSignatureAudio,EStateJournalSystem, StateEvent, FChapterDataPhoto, ItemDataPhoto);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RISINGSIGNAL_API UJournalSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UJournalSystem();

    // TODO ++array.Empty()

    // Add element to Struct's array
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Add")
    void AddNoteItem(FInteractItemNote &NewNoteObj);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Add")
    void AddAudioItem(FInteractItemAudio &NewAudioObj);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Add")
    void AddPhotoItem(FInteractItemPhoto &NewPhotoObj);

    // Get State
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    void GetNextJournalState(EStateJournalSystem NextState);

    // // Get element by Index
    // UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    // FInteractItemNote GetNoteObjByIndex(int32 LevelIndex, int32 NotesIndex);
    //
    // UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    // FInteractItemAudio GetAudioObjByIndex(int32 LevelIndex, int32 NotesIndex);
    //
    // UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    // FInteractItemPhoto GetPhotoObjByIndex(int32 LevelIndex, int32 NotesIndex);

    // Get array size
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    int32 GetArrNoteObjSize();

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    int32 GetArrAudioObjSize();

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    int32 GetArrPhotoObjSize();

    // Get array
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    TArray<FInteractItemNote> GetAllNote();
    
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    TArray<FInteractItemAudio> GetAllAudio();
    
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    TArray<FInteractItemPhoto> GetAllPhoto();

    // // Get Chapter's Index
    // UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    // int32 GetNoteChapterIndex(FString NoteName);

    // UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    // int32 GetAudioChapterIndex(FString AudioName);
    //
    // UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    // int32 GetPhotoChapterIndex(FString PhotoName);

    // Delete element by Index
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Delete")
    void DeleteNoteObjByIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Delete")
    void DeleteAudioObjByIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Delete")
    void DeletePhotoObjByIndex(int32 Index);

    //Delegate signature on update events
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UJournalSystem|Signature")
    FJournalSystemUpdateSignature OnJournalSystemUpdate;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UJournalSystem|Signature")
    FJournalSystemUpdateSignature OnJournalSystemUpdateNote;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UJournalSystem|Signature")
    FJournalSystemUpdateSignature OnJournalSystemUpdatePhoto;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UJournalSystem|Signature")
    FJournalSystemUpdateSignature OnJournalSystemUpdateAudio;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    EStateJournalSystem StateJournalSystem = EStateJournalSystem::Note;

    UPROPERTY()
    TArray<FInteractItemNote> ArrNoteObj;

    UPROPERTY()
    TArray<FInteractItemAudio> ArrAudioObj;

    UPROPERTY()
    TArray<FInteractItemPhoto> ArrPhotoObj;
};