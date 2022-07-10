// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/Widgets/InteractWidget.h"

void UInteractWidget::StartAnimation()
{
    PlayAnimation(this->StartAnim);
}

void UInteractWidget::EndAnimation()
{
    PlayAnimation(this->EndAnim,0,1, EUMGSequencePlayMode::Forward,-1,false);
}

void UInteractWidget::SetText(const FText& NewText) const
{
    this->InteractTextBlock->SetText(NewText);
}
