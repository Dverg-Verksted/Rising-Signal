/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#include "Game/UIMenu/UI/MSWelcomeUserWidget.h"
#include "Game/UIMenu/Base/HUDGameMode/MSGameMode.h"

void UMSWelcomeUserWidget::SetupStateWidget(const EMSStateObject NewState)
{
    Super::SetupStateWidget(NewState);
    if (NewState == EMSStateObject::Active)
    {
        StartLogo();
        GetGameMode()->ChangeMenuStateTimer(EMSMenuState::MainMenu, GetStartAnim()->GetEndTime());
    }
}

void UMSWelcomeUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}
