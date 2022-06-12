// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Entities/JournalAudioEntity.h"
#include "Entities/JournalNoteEntity.h"
#include "Entities/JournalPhotoEntity.h"
#include "JournalSystem.generated.h"

UENUM(BlueprintType)
enum class EStateJournalSystem: uint8
{
	Note,
	Photo,
	Audio
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RISINGSIGNAL_API UJournalSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UJournalSystem();

	//TODO Обработать, хранить существуюие объекты (mb. APPEND?)/Empty()?
	//Добавлять массив или множество? Add or AddUnique???

	//Add element to array
	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	void AddNoteItem(UJournalNoteEntity* NewNoteObj);
	
	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	void AddAudioItem(UJournalAudioEntity* NewAudioObj);
	
	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	void AddPhotoItem(UJournalPhotoEntity* NewPhotoObj);

	//Get State
	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	void GetNextJournalState(EStateJournalSystem NextState);

	//Get element by Index
	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	UJournalNoteEntity* GetNoteObjByIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	UJournalAudioEntity* GetAudioObjByIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	UJournalPhotoEntity* GetPhotoObjByIndex(int32 Index);

	//Get array size
	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	int32 GetArrNoteObjSize();

	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	int32 GetArrAudioObjSize();

	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	int32 GetArrPhotoObjSize();

	//Delete element by Index
	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	void DeleteNoteObjByIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	void DeleteAudioObjByIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category="UJournalSystem|Action")
	void DeletePhotoObjByIndex(int32 Index);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	EStateJournalSystem StateJournalSystem = EStateJournalSystem::Note;
	
	UPROPERTY()
	TArray<UJournalNoteEntity*> ArrNoteObj;
		
	UPROPERTY()
	TArray<UJournalAudioEntity*> ArrAudioObj;
	
	UPROPERTY()
	TArray<UJournalPhotoEntity*> ArrPhotoObj;


public:
};
