// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Game/HUD/Player/Base/HUDWidgetBase.h"
#include "GamePauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API UGamePauseWidget : public UHUDWidgetBase
{
	GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
    UButton* ContinueBtn;
    
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* QuitBtn;

    UPROPERTY(Transient, meta = (BindWidget))
    UButton* Checkpoint_Btn;

    UPROPERTY(Transient, meta = (BindWidget))
    UButton* ResetLevel_Btn;

protected:

    virtual void NativeConstruct() override;

private:

    UFUNCTION()
    void QuitGame();

    UFUNCTION()
    void LoadLevelCheckPoint(bool FromBegin);

    UFUNCTION()
    void LoadLevelBegin();

    UFUNCTION()
    void LoadLevelSave();
    
};
