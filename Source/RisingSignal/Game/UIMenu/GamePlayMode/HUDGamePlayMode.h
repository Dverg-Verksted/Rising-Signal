/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HUDGamePlayMode.generated.h"

/**
 * @class 
 */
UCLASS()
class RISINGSIGNAL_API AHUDGamePlayMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    // Get singleton class
    static AHUDGamePlayMode* Get(UWorld* World) { return (World) ? Cast<AHUDGamePlayMode>(World->GetAuthGameMode()) : nullptr; }
};
