// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Entities/JournalAudioEntity.h"
#include "Entities/JournalNoteEntity.h"
#include "Entities/JournalPhotoEntity.h"
#include "Library/RSFunctionLibrary.h"
#include "JournalSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJournalSystemUpdateSignature);

UENUM(BlueprintType)
enum class EStateJournalSystem : uint8
{
    Note,
    Photo,
    Audio
};

USTRUCT(BlueprintType)
struct FChapterDataNote
{
    GENERATED_BODY()

    FString ChapterName = TEXT("Map_TestInventory");

    UPROPERTY()
    TArray<UJournalNoteEntity*> ArrNote;
};

USTRUCT(BlueprintType)
struct FChapterDataAudio
{
    GENERATED_BODY()

    FString ChapterName = TEXT("Map_TestInventory");

    UPROPERTY()
    TArray<UJournalAudioEntity*> ArrAudio;
};

USTRUCT(BlueprintType)
struct FChapterDataPhoto
{
    GENERATED_BODY()

    FString ChapterName = TEXT("Map_TestInventory");

    UPROPERTY()
    TArray<UJournalPhotoEntity*> ArrPhoto;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RISINGSIGNAL_API UJournalSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UJournalSystem();

    // TODO ++array.Empty()

    // Add element to Struct's array
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Add")
    void AddNoteItem(UJournalNoteEntity* NewNoteObj);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Add")
    void AddAudioItem(UJournalAudioEntity* NewAudioObj);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Add")
    void AddPhotoItem(UJournalPhotoEntity* NewPhotoObj);

    // Get State
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    void GetNextJournalState(EStateJournalSystem NextState);

    // Get element by Index
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    UJournalNoteEntity* GetNoteObjByIndex(int32 LevelIndex, int32 NotesIndex);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    UJournalAudioEntity* GetAudioObjByIndex(int32 LevelIndex, int32 AudioIndex);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    UJournalPhotoEntity* GetPhotoObjByIndex(int32 LevelIndex, int32 PhotoIndex);

    // Get array size
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    int32 GetArrNoteObjSize();

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    int32 GetArrAudioObjSize();

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    int32 GetArrPhotoObjSize();

    // Get array
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    TArray<FChapterDataNote> GetAllNote();
    
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    TArray<FChapterDataAudio> GetAllAudio();
    
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    TArray<FChapterDataPhoto> GetAllPhoto();

    // Get Chapter's Index
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    int32 GetNoteChapterIndex(FString NoteName);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    int32 GetAudioChapterIndex(FString AudioName);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Get")
    int32 GetPhotoChapterIndex(FString PhotoName);

    // Delete element by Index
    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Delete")
    void DeleteNoteObjByIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Delete")
    void DeleteAudioObjByIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "UJournalSystem|Action|Delete")
    void DeletePhotoObjByIndex(int32 Index);

    //Delegate signature on update events
    UPROPERTY(BlueprintAssignable, Category="UJournalSystem|Signature")
    FJournalSystemUpdateSignature OnJournalSystemUpdate;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    EStateJournalSystem StateJournalSystem = EStateJournalSystem::Note;

    UPROPERTY()
    TArray<FChapterDataNote> ArrNoteObj;

    UPROPERTY()
    TArray<FChapterDataAudio> ArrAudioObj;

    UPROPERTY()
    TArray<FChapterDataPhoto> ArrPhotoObj;
};