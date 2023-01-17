// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/HUD/Debug/DebugHUD.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"
//#include "Player/NewTestPlayer/RSBaseCharacter.h"
#include "GameHUD.generated.h"

class ARSGamePLayer;
/**
 * @class Base game play HUD
 */
UCLASS()
class RISINGSIGNAL_API AGameHUD : public ADebugHUD
{
    GENERATED_BODY()

protected:
    // Draw the HUD.
    virtual void DrawHUD() override;

    /** Overridable native event for when play begins for this actor. */
    virtual void BeginPlay() override;

private:
    // Current pointer on Character
    UPROPERTY()
    ARSBaseCharacter* GamePlayer;
};
