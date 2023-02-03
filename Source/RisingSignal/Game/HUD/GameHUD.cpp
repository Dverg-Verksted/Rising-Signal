// It is owned by the company Dverg Verksted.

#include "Game/HUD/GameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/RSGamePLayer.h"
#include "Player/Base/HUDPlayerInterfaceWidget.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"
#include "Player/Pause/GamePauseWidget.h"
//#include "Player/NewTestPlayer/RSBaseCharacter.h"

void AGameHUD::DrawHUD()
{
    Super::DrawHUD();

    if (EnableDebugHUD)
    {
        // Example for debug screen messages
        // AddText(TEXT("Test function AddText"), FText::FromString("This AddText"));
        // AddFloat(TEXT("Test function AddFloat"), 1.488f);
        // AddInt(TEXT("Test function AddInt"), 228);
        // AddBool(TEXT("Test function AddBool"), false);
        // AddBool(TEXT("Test function AddBool"), true);
    }
}

void AGameHUD::BeginPlay()
{
    Super::BeginPlay();

    this->GamePlayer = Cast<ARSBaseCharacter>(GetOwningPlayerController()->GetPawn());
    if(!CHECKMS(this->GamePlayer != nullptr, "GamePlayer is Nullptr")) return;

    this->GamePlayerController = Cast<ARSGamePlayerController>(GetOwningPlayerController());
    if(!CHECKMS(this->GamePlayerController != nullptr, "GamePlayerController is Nullptr")) return;

    PlayerInterfaceWidget = CreateWidget<UHUDPlayerInterfaceWidget>(GetWorld(), PlayerInterfaceWidgetClass);
    PlayerInterfaceWidget->AddToViewport();

    GamePlayerController->OnGamePaused.AddDynamic(this, &AGameHUD::PauseViewON);

    GamePauseWidget = CreateWidget<UGamePauseWidget>(GetWorld(), GamePauseWidgetClass);
    GamePauseWidget->AddToViewport();
    GamePauseWidget->SetVisibility(ESlateVisibility::Hidden);
    
    GamePauseWidget->ContinueBtn->OnClicked.AddDynamic(this, &AGameHUD::PauseViewOff);
    
}

void AGameHUD::PauseViewON()
{
    LOG_RS(ELogRSVerb::Display, TEXT("PAUSE VIEW ON"));
    PlayerInterfaceWidget->SetVisibility(ESlateVisibility::Hidden);
    GamePauseWidget->SetVisibility(ESlateVisibility::Visible);
    GamePlayerController->SetShowMouseCursor(true);
    GamePlayerController->SetInputMode(FInputModeUIOnly());
    
}

void AGameHUD::PauseViewOff()
{
    PlayerInterfaceWidget->SetVisibility(ESlateVisibility::Visible);
    GamePauseWidget->SetVisibility(ESlateVisibility::Hidden);
    GamePlayerController->SetShowMouseCursor(false);
    GamePlayerController->SetInputMode(FInputModeGameOnly());
    
    UGameplayStatics::SetGamePaused(GamePlayerController, false);
}

