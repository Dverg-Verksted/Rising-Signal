// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/HUD/GameHUD.h"
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

    virtual void StartPlay() override;

    UFUNCTION(BlueprintCallable)
    void CheckpointReached(AActor* ReachedActor);

    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

    UFUNCTION(BlueprintCallable)
    void LoadCheckpoint();

    UFUNCTION(BlueprintCallable)
    void LoadLevel(FName LevelName, bool FromBegin);

    void RestartLevel();

    

protected:
    virtual void BeginPlay() override;

    void GameStarted();

private:
    // Current pointer on ARSGamePlayerController
    UPROPERTY()
    ARSGamePlayerController* PC;

    // Current pointer on AGameHUD
    UPROPERTY()
    AGameHUD* GameHUD;
    
    UPROPERTY()
    class URSSaveGameSubsystem* SG;

    bool bGameWasLoaded = false;
};
