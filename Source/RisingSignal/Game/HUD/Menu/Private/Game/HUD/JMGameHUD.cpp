/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#include "Game/HUD/JMGameHUD.h"
#include "Game/GamePlayMode.h"
#include "Library/JamMSFunctionLibrary.h"

void AJMGameHUD::DrawHUD()
{
    Super::DrawHUD();

    if (this->GameMode && EnableJMDebugHUD)
    {
        AddText(TEXT("Test function AddText"), FText::FromString("This AddText"));
        AddFloat(TEXT("Test function AddFloat"), 1.488f);
        AddInt(TEXT("Test function AddInt"), 228);
        AddBool(TEXT("Test function AddBool"), false);
        AddBool(TEXT("Test function AddBool"), true);
    }
}

void AJMGameHUD::BeginPlay()
{
    Super::BeginPlay();

    this->GameMode = AGamePlayMode::Get(GetWorld());
    if (!CHECK(this->GameMode != nullptr, FString("Game mode is nullptr"))) return;
}
