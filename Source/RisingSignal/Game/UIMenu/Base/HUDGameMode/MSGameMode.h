/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#pragma once

#include "CoreMinimal.h"
#include "Game/UIMenu/Base/MSDataTypes.h"
#include "GameFramework/GameModeBase.h"
#include "MSGameMode.generated.h"

// Forward declare
class UMSGameInstance;

/**
 * @class Game mode for system menu and level menu
 */
UCLASS()
class RISINGSIGNAL_API AMSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

    // Construct default
    AMSGameMode();
    
    // Singleton
    UFUNCTION(BlueprintCallable, Category = "AMSGameMode|Singleton")
    static AMSGameMode* Get(UWorld* World) { return (World) ? Cast<AMSGameMode>(World->GetAuthGameMode()) : nullptr; }
    
    // Signature on change Menu State
    UPROPERTY(BlueprintAssignable)
    FOnMenuStateChangeSignature OnMenuStateChanged;

    /**
     * @public Change current menu state on new state
     * @param1 EJamMSMenuState
     **/
    UFUNCTION(BlueprintCallable, Category = "AJamMSGameMode|Action")
    void ChangeMenuState(EMSMenuState NewState);

    /**
     * @public Change current menu state on new state for Timer
     * @param1 EJamMSMenuState
     * @param2 float
     **/
    UFUNCTION(BlueprintCallable, Category = "AJamMSGameMode|Action")
    void ChangeMenuStateTimer(EMSMenuState NewState, float RateTime);

    EMSMenuState GetMenuState() const { return this->MenuState; }
    
#pragma region ExecCheat
    
    /**
     * @public @exec Change the status of displaying logs on the screen
     * @param1 EStateShowLog
     **/
    UFUNCTION(Exec, Category = "AJamMSGameMode|Exec")
    static void ChangeStateShowLog(const EMSStateShowLog NewState);
    
#pragma endregion 
    
protected:
    
    /** Overridable native event for when play begins for this actor. */
    virtual void BeginPlay() override;
    
private:
    // Current pointer on UJamMSGameInstance class
    UPROPERTY()
    UMSGameInstance* GameInst;
    
    // Current pointer on UGameUserSettings class
    UPROPERTY()
    UGameUserSettings* UserSettings;

    // Current menu state in Menu Level
    EMSMenuState MenuState = EMSMenuState::WaitToStart;

    
};
