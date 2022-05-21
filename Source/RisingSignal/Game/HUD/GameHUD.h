// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/DebugHUD.h"
#include "GameHUD.generated.h"

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
	
};
