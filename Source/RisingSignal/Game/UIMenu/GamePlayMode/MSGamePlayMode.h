/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MSGamePlayMode.generated.h"

/**
 * @class 
 */
UCLASS()
class RISINGSIGNAL_API AMSGamePlayMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    // Get singleton class
    static AMSGamePlayMode* Get(UWorld* World) { return (World) ? Cast<AMSGamePlayMode>(World->GetAuthGameMode()) : nullptr; }
};
