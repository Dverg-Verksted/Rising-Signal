// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/GameHUD.h"
#include "GameFramework/GameModeBase.h"
#include "RSGameMode.generated.h"

class ARSGamePlayerController;
/**
 * @class The main game mode
 */
UCLASS()
class RISINGSIGNAL_API ARSGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
#pragma region Cheat

    /**
     * @public Change state param DebugHUD from GameHUD
     **/
    UFUNCTION(Exec)
    void ChangeStateDebugHUD(const bool NewState) const
    {
        if (IsValid(this->GameHUD)) this->GameHUD->ChangeEnableDebugHUD(NewState);
    }

#pragma endregion

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    void OnChecpointReached(AActor* ReachedActor);

    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

protected:
    virtual void BeginPlay() override;

private:
    // Current pointer on ARSGamePlayerController
    UPROPERTY()
    ARSGamePlayerController* PC;

    // Current pointer on AGameHUD
    UPROPERTY()
    AGameHUD* GameHUD;
};
