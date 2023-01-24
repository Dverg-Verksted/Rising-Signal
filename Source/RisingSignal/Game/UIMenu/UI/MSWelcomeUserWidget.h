/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#pragma once

#include "CoreMinimal.h"
#include "Game/UIMenu/UI/MSMenuUserWidgetBase.h"
#include "MSWelcomeUserWidget.generated.h"

// Forward declare
class UImage;

/**
 * @class The usual widget for introductory pictures
 */
UCLASS()
class RISINGSIGNAL_API UMSWelcomeUserWidget : public UMSMenuUserWidgetBase
{
	GENERATED_BODY()

public:
    /**
     * @public Setup new state widget (Intended for override)
     * @param1 EMSStateObject
     **/
    virtual void SetupStateWidget(const EMSStateObject NewState) override;
    
protected:
    // Native handling for SObjectWidget
    virtual void NativeOnInitialized() override;

    // Blueprint native event
    UFUNCTION(BlueprintImplementableEvent)
    void StartLogo();

private:
    // A picture that will act as a greeting
    UPROPERTY(Transient, meta = (BindWidget))
    UImage* WelcomeImage;
};
