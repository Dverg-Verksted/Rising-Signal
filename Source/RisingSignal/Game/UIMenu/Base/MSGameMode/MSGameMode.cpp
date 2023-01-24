/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#include "MSGameMode.h"
#include "Game/UIMenu/Base/MSGameMode/MSGameMode.h"
#include "Game/UIMenu/Base/MSGameInstance/MSGameInstance.h"
#include "Game/UIMenu/Base/MenuPlayerController/MSPlayerController.h"
#include "GameFramework/GameUserSettings.h"
#include "Game/UIMenu/MSHUD.h"
#include "Game/UIMenu/Library/HUDMSFunctionLibrary.h"

AMSGameMode::AMSGameMode()
{
    LOGJAM(ELogVerb::Display, "Game mode contructor");
    HUDClass = AMSHUD::StaticClass();
    PlayerControllerClass = AMSPlayerController::StaticClass();
}

void AMSGameMode::BeginPlay()
{
    Super::BeginPlay();

    this->GameInst = UMSGameInstance::Get(GetWorld());
    if (!CHECK(this->GameInst != nullptr, "Game instance is nullptr")) return;
    
    this->UserSettings = UGameUserSettings::GetGameUserSettings();
    if (!CHECK(this->UserSettings != nullptr, "User settings is nullptr")) return;

    ChangeMenuStateTimer(EMSMenuState::WelcomeToGame, 0.1f);
}

void AMSGameMode::ChangeMenuState(EMSMenuState NewState)
{
    if (this->MenuState == NewState)
    {
        LOGJAM(ELogVerb::Warning, FString::Printf(TEXT("Current menu state equal new state: %s"),
            *UEnum::GetValueAsString(NewState)));
        return;
    }

    LOGJAM(ELogVerb::Display, FString::Printf(TEXT("New menu state: %s"), *UEnum::GetValueAsString(NewState)));
    this->MenuState = NewState;
    this->OnMenuStateChanged.Broadcast(NewState);
}

void AMSGameMode::ChangeMenuStateTimer(EMSMenuState NewState, float RateTime)
{
    if (RateTime <= 0.0f)
    {
        LOGJAM(ELogVerb::Warning, FString::Printf(TEXT("Call function with rate timer: %f <= 0.0"), RateTime));
        return;
    }
    
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &AMSGameMode::ChangeMenuState, NewState);
    GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, RateTime, false);
}

#pragma region ExecCheatFunc
void AMSGameMode::ChangeStateShowLog(const EMSStateShowLog NewState)
{
    StateShowLog = NewState;
}

#pragma endregion
