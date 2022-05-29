// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "JournalBaseEntity.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class RISINGSIGNAL_API UJournalBaseEntity : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Journal", meta=(DisplayName="Тип сущности Журнала", ToolTip="Требуется точное определение сущности"))
	FPrimaryAssetType JournalType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Journal", meta=(DisplayName="Заголовок Журнала", ToolTip="Заголовок присутствует в каждой сущности журнала"))
	FText JournalHeader;

	UFUNCTION(BlueprintCallable, Category = "Journal", meta=(DisplayName="Метод для получения ID сущности журнала", ToolTip="Метод для получения ID сущности журнала"))
	FString GetIdentifierString();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
