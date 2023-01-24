/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/
#include "Game/UIMenu/UI/MSMainMenuUserWidget.h"
#include "Game/UIMenu/Base/MSGameMode/MSGameMode.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMSMainMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->MSNewGameButton->OnClicked.AddDynamic(this, &UMSMainMenuUserWidget::NewGame);
    this->MSSettingsButton->OnClicked.AddDynamic(this, &UMSMainMenuUserWidget::SwitchToSettings);
    this->MSCreditsButton->OnClicked.AddDynamic(this, &UMSMainMenuUserWidget::SwitchToCredits);
    this->MSExitButton->OnClicked.AddDynamic(this, &UMSMainMenuUserWidget::CloseGame);
}

void UMSMainMenuUserWidget::NewGame()
{
    if (GetStateButton() == EMSStateObject::Inactive) return;
    ShowAnim(this->MSNewGameButtonAnim);
    GetGameMode()->ChangeMenuStateTimer(EMSMenuState::Loading, this->MSNewGameButtonAnim->GetEndTime());
}

void UMSMainMenuUserWidget::SwitchToSettings()
{
    if (GetStateButton() == EMSStateObject::Inactive) return;
    ShowAnim(this->MSSettingsButtonAnim);
    GetGameMode()->ChangeMenuStateTimer(EMSMenuState::Settings, this->MSSettingsButtonAnim->GetEndTime());
}

void UMSMainMenuUserWidget::SwitchToCredits()
{
    if (GetStateButton() == EMSStateObject::Inactive) return;
    ShowAnim(this->MSCreditsButtonAnim);
    GetGameMode()->ChangeMenuStateTimer(EMSMenuState::Credits, this->MSCreditsButtonAnim->GetEndTime());
}

void UMSMainMenuUserWidget::CloseGame()
{
    if (GetStateButton() == EMSStateObject::Inactive) return;
    LOGMENU(ELogVerb::Display, "Goodbye!");
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
