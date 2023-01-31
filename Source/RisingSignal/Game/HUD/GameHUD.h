  // It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/DebugHUD.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"
#include "GameHUD.generated.h"

  class UHUDPlayerInterfaceWidget;
    class UGamePauseWidget;
  class ARSGamePLayer;


/**
 * @class Base game play HUD
 */
UCLASS()
class RISINGSIGNAL_API AGameHUD : public ADebugHUD
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UHUDPlayerInterfaceWidget> PlayerInterfaceWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UGamePauseWidget> GamePauseWidgetClass;

    UPROPERTY(BlueprintReadOnly)
    ARSGamePlayerController* GamePlayerController;

    UPROPERTY(BlueprintReadWrite)
    UHUDPlayerInterfaceWidget* PlayerInterfaceWidget;

    UPROPERTY(BlueprintReadOnly)
    UGamePauseWidget* GamePauseWidget;
    
protected:
    // Draw the HUD.
    virtual void DrawHUD() override;

    /** Overridable native event for when play begins for this actor. */
    virtual void BeginPlay() override;

    UFUNCTION()
    void PauseViewON();

    UFUNCTION()
    void PauseViewOff();
    
private:
    // Current pointer on Character
    UPROPERTY()
    ARSBaseCharacter* GamePlayer;
    
};
