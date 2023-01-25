/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#include "Game/UIMenu/UI/MSLoadingUserWidget.h"
#include "Game/UIMenu/Base/MSGameInstance/MSGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMSLoadingUserWidget::SetupStateWidget(const EMSStateObject NewState)
{


    Super::SetupStateWidget(NewState);

    if (NewState == EMSStateObject::Active)
    {
        UGameplayStatics::OpenLevel(GetWorld(), GetGameInst()->GetContinueLevelName(), true, L"LoadCheckpoint=True?Save=False");
    }
}

void UMSLoadingUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}
