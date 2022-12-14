// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RSSaveGameSettings.generated.h"


class UDataTable;


UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Save Game Settings"))
class RISINGSIGNAL_API URSSaveGameSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    /* Default slot name if UI doesn't specify any */
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
    FString SaveSlotName;

    /* Soft path must be converted to asset before use */
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", AdvancedDisplay)
    TSoftObjectPtr<UDataTable> DummyTablePath;

    URSSaveGameSettings();
};
