// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Game/HUD/Player/Base/HUDWidgetBase.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API UInventoryWidget : public UHUDWidgetBase
{
	GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

private:

    UPROPERTY(Transient, meta = (BindWidget))
    UButton* BtnClose;
    
};
