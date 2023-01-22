/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#include "Game/UIMenu/UI/MSMenuUserWidgetBase.h"
#include "Game/UIMenu/Base/HUDGameInstance/MSGameInstance.h"
#include "Game/UIMenu/Base/HUDGameMode/MSGameMode.h"
#include "Game/UIMenu/Library/HUDMSFunctionLibrary.h"

void UMSMenuUserWidgetBase::Print_Menu(ELogVerb TypeVerb, FString Str, int Line, const char* Function) const
{
    UMSFunctionLibrary::Print_Log(TypeVerb, FString::Printf(TEXT("Name widget: %s | "), *GetName()) + Str, Line, Function);
}

void UMSMenuUserWidgetBase::ShowAnim(UWidgetAnimation* Anim)
{
    SetupStateButton(EMSStateObject::Inactive);
    LOGMENU(ELogVerb::Display, FString::Printf(TEXT("Show animation: %s"), *Anim->GetName()));
    PlayAnimation(Anim);

    if (this->StateWidget == EMSStateObject::Active)
    {
        // Reset to active button
        FTimerHandle TimerHandle;
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindLambda([this]
        {
            SetupStateButton(EMSStateObject::Active);
        });
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Anim->GetEndTime(), false);
        
        LOGMENU(ELogVerb::Display, FString::Printf(TEXT("Restart button state via: %f"), Anim->GetEndTime()));
    }
}

void UMSMenuUserWidgetBase::ShowAnimTimer(UWidgetAnimation* Anim, float RateTime)
{
    if (GetWorld()->GetTimerManager().TimerExists(this->TimerHandleShowAnim))
    {
        LOGMENU(ELogVerb::Warning, FString::Printf(TEXT("Timer Handle: %s is exist"), *this->TimerHandleShowAnim.ToString()));
        return;
    }

    if (RateTime <= 0.0f)
    {
        LOGMENU(ELogVerb::Warning, FString::Printf(TEXT("Call function with rate timer: %f <= 0.0"), RateTime));
        return;
    }
    
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UMSMenuUserWidgetBase::ShowAnim, Anim);
    GetWorld()->GetTimerManager().SetTimer(this->TimerHandleShowAnim, TimerDelegate, RateTime, false);

    LOGMENU(ELogVerb::Display, FString::Printf(TEXT("Show Animation: %s via: %f"), *Anim->GetName(), Anim->GetEndTime()));
}

void UMSMenuUserWidgetBase::SetupStateButton(EMSStateObject NewState)
{
    if (this->StateWidget == EMSStateObject::Inactive) return;
    LOGMENU(ELogVerb::Display, FString::Printf(TEXT("New State button: %s"), *UEnum::GetValueAsString(NewState)));
    this->StateButton = NewState;
}

void UMSMenuUserWidgetBase::SetupStateButtonTimer(EMSStateObject NewState, float RateTime)
{
    if (GetWorld()->GetTimerManager().TimerExists(this->TimerHandleButton))
    {
        LOGMENU(ELogVerb::Warning, FString::Printf(TEXT("Timer Handle: %s is exist"), *this->TimerHandleButton.ToString()));
        return;
    }

    if (RateTime <= 0.0f)
    {
        LOGMENU(ELogVerb::Warning, FString::Printf(TEXT("Call function with rate timer: %f <= 0.0"), RateTime));
        return;
    }

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UMSMenuUserWidgetBase::SetupStateButton, NewState);
    GetWorld()->GetTimerManager().SetTimer(this->TimerHandleShowAnim, TimerDelegate, RateTime, false);
}

void UMSMenuUserWidgetBase::SetupStateWidget(const EMSStateObject NewState)
{
    LOGMENU(ELogVerb::Display, FString::Printf(TEXT("New State widget: %s"), *UEnum::GetValueAsString(NewState)));
    this->StateWidget = NewState;
    SetupStateButton(NewState);
}

void UMSMenuUserWidgetBase::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    LOGMENU(ELogVerb::Display, "Native On Initialized");
    
    this->GameMode = AMSGameMode::Get(GetWorld());
    if (!CHECK(this->GameMode != nullptr, "Game mode is nullptr")) return;

    this->GameInst = UMSGameInstance::Get(GetWorld());
    if (!CHECK(this->GameInst != nullptr, "Game instance is nullptr")) return;
    
}
