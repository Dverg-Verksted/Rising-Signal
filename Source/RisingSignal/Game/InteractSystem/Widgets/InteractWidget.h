// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "InteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    void StartAnimation();
    void EndAnimation();
    void SetText(const FText& NewText) const;

private:

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* StartAnim;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* EndAnim;

    UPROPERTY(Transient, meta = (BindWidget))
    UTextBlock* InteractTextBlock;

    
};
