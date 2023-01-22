/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#include "Game/UIMenu/UI/MSCreditsUserWidget.h"
#include "Game/UIMenu/Base/HUDGameMode/MSGameMode.h"
#include "Game/UIMenu/Base/MSDataTypes.h"
#include "Components/Button.h"

void UMSCreditsUserWidget::SetupStateWidget(const EMSStateObject NewState)
{
    Super::SetupStateWidget(NewState);
    if (NewState == EMSStateObject::Active)
    {
        PlayAnimation(this->LoopAnim, 0, 0);
    }
    else if (NewState == EMSStateObject::Inactive)
    {
        StopAnimation(this->LoopAnim);
    }
}

void UMSCreditsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->BackButton->OnClicked.AddDynamic(this, &UMSCreditsUserWidget::ChangeToMainMenu);
}

void UMSCreditsUserWidget::ChangeToMainMenu()
{
    if (GetStateButton() == EMSStateObject::Inactive) return;
    ShowAnim(BackButtonAnim);
    GetGameMode()->ChangeMenuStateTimer(EMSMenuState::MainMenu, BackButtonAnim->GetEndTime());
}
