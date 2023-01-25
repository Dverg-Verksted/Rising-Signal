/**
* Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/
#pragma once

#include "CoreMinimal.h"
#include "MSMenuUserWidgetBase.h"
#include "MSNewGameUserWidget.generated.h"


/**
 * @class Plug for loading the level
 */
UCLASS()
class RISINGSIGNAL_API UMSNewGameUserWidget : public UMSMenuUserWidgetBase
{
    GENERATED_BODY()

public:
    /**
     * @public Setup new state widget (Intended for override)
     * @param1 EStateObject
     **/
    virtual void SetupStateWidget(const EMSStateObject NewState) override;

protected:
    // Native handling for SObjectWidget
    virtual void NativeOnInitialized() override;
    
};
