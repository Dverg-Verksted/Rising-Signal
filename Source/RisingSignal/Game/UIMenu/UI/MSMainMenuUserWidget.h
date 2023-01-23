/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/
#pragma once

#include "CoreMinimal.h"
#include "Game/UIMenu/UI/MSMenuUserWidgetBase.h"
#include "MSMainMenuUserWidget.generated.h"

// Forward declare
class UButton;

/**
 * @class Main Menu Widget
 */
UCLASS()
class RISINGSIGNAL_API UMSMainMenuUserWidget : public UMSMenuUserWidgetBase
{
	GENERATED_BODY()

protected:
    // @protected Native handling for SObjectWidget
    virtual void NativeOnInitialized() override;

private:
    // @private Button to switch to a new game
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* MSNewGameButton;
    // @private Animation Button to switch to a new game
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* MSNewGameButtonAnim;
    
    // @private Button to switch to a settings
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* MSSettingsButton;
    // @private Animation Button to switch to a settings
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* MSSettingsButtonAnim;

    // @private Button to switch to a credits game
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* MSCreditsButton;
    // @private Animation Button to switch to a credits game
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* MSCreditsButtonAnim;

    // @private Button to close game
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* MSExitButton;

    /**
     * @private Setup a new game
     **/
    UFUNCTION()
    void NewGame();

    /**
     * @private Setup a Settings state
     **/
    UFUNCTION()
    void SwitchToSettings();

    /**
     * @private Setup a Credits state
     **/
    UFUNCTION()
    void SwitchToCredits();

    /**
     * @private Close game
     **/
    UFUNCTION()
    void CloseGame();
};
