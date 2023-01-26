/**
* Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#include "Game/UIMenu/UI/MSNewGameUserWidget.h"
#include "Game/UIMenu/Base/MSGameInstance/MSGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMSNewGameUserWidget::SetupStateWidget(const EMSStateObject NewState)
{
    Super::SetupStateWidget(NewState);
    if (NewState == EMSStateObject::Active)
    {
        UGameplayStatics::OpenLevel(GetWorld(), GetGameInst()->GetNameStartLevel(), true, L"LoadCheckpoint=False?Save=True");
    }
}

void UMSNewGameUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}
