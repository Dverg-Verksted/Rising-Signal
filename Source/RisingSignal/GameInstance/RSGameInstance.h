// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RSGameInstance.generated.h"

/**
 * @class Working game instance
 */
UCLASS()
class RISINGSIGNAL_API URSGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // Singleton
    UFUNCTION(BlueprintCallable, Category = "URSGameInstance|Singleton")
    static URSGameInstance* Get(UWorld* World) { return (World) ? Cast<URSGameInstance>(World->GetGameInstance()) : nullptr; }
    
};
