/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MSPlayerController.generated.h"

/**
 * @class Menu Player Controller
 */
UCLASS()
class RISINGSIGNAL_API AMSPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
};
