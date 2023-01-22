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
    UButton* NewGameButton;
    // @private Animation Button to switch to a new game
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* NewGameButtonAnim;
    
    // @private Button to switch to a settings
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* SettingsButton;
    // @private Animation Button to switch to a settings
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* SettingsButtonAnim;

    // @private Button to switch to a credits game
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* CreditsButton;
    // @private Animation Button to switch to a credits game
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* CreditsButtonAnim;

    // @private Button to close game
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* ExitButton;

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
