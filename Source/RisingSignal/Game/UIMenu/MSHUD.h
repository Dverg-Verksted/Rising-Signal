/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#pragma once

#include "CoreMinimal.h"
#include "Game/UIMenu/Base/MSDataTypes.h"
#include "GameFramework/HUD.h"
#include "MSHUD.generated.h"

// Forward declare
class UMSMenuUserWidgetBase;
class AMSGameMode;

/**
 * @class Menu HUD
 */
UCLASS()
class RISINGSIGNAL_API AMSHUD : public AHUD
{
	GENERATED_BODY()
 
public:

    /**
     * @public Getting current pointer on AMSGameMode
     * @return AJamMSGameMode
     **/
    AMSGameMode* GetGameMode() const { return this->GameMode; }
    
protected:
    /** Overridable native event for when play begins for this actor. */
    virtual void BeginPlay() override;
    
private:

    // @private Current pointer on AMSGameMode class
    UPROPERTY()
    AMSGameMode* GameMode;
    
    // @private Current sub class on UMenuUserWidgetBase from WelcomeHudWidgetClass
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMSMenuUserWidgetBase> WelcomeHudWidgetClass;

    // @private Current sub class on UMenuUserWidgetBase from MenuHudWidgetClass
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMSMenuUserWidgetBase> MenuHudWidgetClass;

    // @private Current sub class on UMenuUserWidgetBase from OptionsHudWidgetClass
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMSMenuUserWidgetBase> SettingsHudWidgetClass;

    // @private Current sub class on UMenuUserWidgetBase from CreditsHudWidgetClass
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMSMenuUserWidgetBase> CreditsHudWidgetClass;

    // @private Current sub class on UMenuUserWidgetBase from CreditsHudWidgetClass
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMSMenuUserWidgetBase> LoadingHudWidgetClass;

    // @private Current sub class on UMenuUserWidgetBase from CreditsHudWidgetClass
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMSMenuUserWidgetBase> NewGameHudWidgetClass;

    // @private Container for storing widgets
    UPROPERTY()
    TMap<EMSMenuState, UMSMenuUserWidgetBase*> MenuWidgets;

    // @private Current pointer on UMenuUserWidgetBase in visible on HUD
    UPROPERTY()
    UMSMenuUserWidgetBase* MenuWidget = nullptr;

    /**
     * @private Registers the new state of the system menu
     * @param1 EMSMenuState
     **/
    UFUNCTION()
    void OnMenuStateChanged(EMSMenuState NewState);

    /**
     * @private Setup new widget visible
     * @param1 UMenuUserWidgetBase
     **/
    UFUNCTION()
    void SetupNewWidget(UMSMenuUserWidgetBase* Widget);

    /**
     * @private Setup new widget visible
     * @param1 UMenuUserWidgetBase
     **/
    void SwitchWidget(UMSMenuUserWidgetBase* OldWidget, UMSMenuUserWidgetBase* NewWidget);
    
};

