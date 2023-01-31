// It is owned by the company Dverg Verksted.


#include "Game/HUD/Player/Pause/GamePauseWidget.h"

#include "Game/GameModes/RSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Library/RSFunctionLibrary.h"

void UGamePauseWidget::NativeConstruct()
{
    Super::NativeConstruct();

    QuitBtn->OnClicked.AddDynamic(this, &UGamePauseWidget::QuitGame);
    Checkpoint_Btn->OnClicked.AddDynamic(this, &UGamePauseWidget::LoadLevelSave);
    ResetLevel_Btn->OnClicked.AddDynamic(this, &UGamePauseWidget::LoadLevelBegin);
    
}

void UGamePauseWidget::QuitGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}

void UGamePauseWidget::LoadLevelCheckPoint(bool FromBegin)
{
    GetGameMode()->LoadLevel(FName(PlayerController->GetWorld()->GetName()), FromBegin, false);
}

void UGamePauseWidget::LoadLevelBegin()
{
    LoadLevelCheckPoint(true);
}

void UGamePauseWidget::LoadLevelSave()
{
    LoadLevelCheckPoint(false);
}

