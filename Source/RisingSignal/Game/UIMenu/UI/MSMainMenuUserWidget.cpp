/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/
#include "Game/UIMenu/UI/MSMainMenuUserWidget.h"
#include "Game/UIMenu/Base/HUDGameMode/MSGameMode.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMSMainMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->NewGameButton->OnClicked.AddDynamic(this, &UMSMainMenuUserWidget::NewGame);
    this->SettingsButton->OnClicked.AddDynamic(this, &UMSMainMenuUserWidget::SwitchToSettings);
    this->CreditsButton->OnClicked.AddDynamic(this, &UMSMainMenuUserWidget::SwitchToCredits);
    this->ExitButton->OnClicked.AddDynamic(this, &UMSMainMenuUserWidget::CloseGame);
}

void UMSMainMenuUserWidget::NewGame()
{
    if (GetStateButton() == EMSStateObject::Inactive) return;
    ShowAnim(this->NewGameButtonAnim);
    GetGameMode()->ChangeMenuStateTimer(EMSMenuState::Loading, this->NewGameButtonAnim->GetEndTime());
}

void UMSMainMenuUserWidget::SwitchToSettings()
{
    if (GetStateButton() == EMSStateObject::Inactive) return;
    ShowAnim(this->SettingsButtonAnim);
    GetGameMode()->ChangeMenuStateTimer(EMSMenuState::Settings, this->SettingsButtonAnim->GetEndTime());
}

void UMSMainMenuUserWidget::SwitchToCredits()
{
    if (GetStateButton() == EMSStateObject::Inactive) return;
    ShowAnim(this->CreditsButtonAnim);
    GetGameMode()->ChangeMenuStateTimer(EMSMenuState::Credits, this->CreditsButtonAnim->GetEndTime());
}

void UMSMainMenuUserWidget::CloseGame()
{
    if (GetStateButton() == EMSStateObject::Inactive) return;
    LOGMENU(ELogVerb::Display, "Goodbye!");
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
