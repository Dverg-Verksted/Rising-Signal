// It is owned by the company Dverg Verksted.

#include "Game/HUD/GameHUD.h"
#include "Player/RSGamePLayer.h"

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
        AddText(TEXT("Inventory"), FText::FromString("Press I - Show Inventory"));
        AddText(TEXT("Journal"), FText::FromString("Press J - Show Journal"));
    }
}

void AGameHUD::BeginPlay()
{
    Super::BeginPlay();

    this->GamePlayer = Cast<ARSGamePLayer>(GetOwningPlayerController()->GetPawn());
    check(this->GamePlayer);
}
