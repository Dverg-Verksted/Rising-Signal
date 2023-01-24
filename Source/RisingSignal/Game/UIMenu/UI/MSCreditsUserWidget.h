/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/
#pragma once

#include "CoreMinimal.h"
#include "MSMenuUserWidgetBase.h"
#include "MSCreditsUserWidget.generated.h"


// Forward declare
class UButton;
class UWidgetAnimation;

/**
 * @class Widget class for displaying creators
 */
UCLASS()
class RISINGSIGNAL_API UMSCreditsUserWidget : public UMSMenuUserWidgetBase
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

private:
    // Looping animation (ZALUPING :D)
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* LoopAnim;

    // @private Button close to Main menu
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* BackButton;
    // @private Button close to Main menu Animation
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* BackButtonAnim;

    /**
     * @private To return to the main menu
     **/
    UFUNCTION()
    void ChangeToMainMenu();
};
