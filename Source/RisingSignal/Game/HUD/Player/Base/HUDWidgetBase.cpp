// It is owned by the company Dverg Verksted.


#include "Game/HUD/Player/Base/HUDWidgetBase.h"

#include "Game/GameModes/RSGameMode.h"
#include "Game/UIMenu/Base/MSGameInstance/MSGameInstance.h"
#include "Game/UIMenu/Base/MSGameMode/MSGameMode.h"
#include "Game/UIMenu/Library/HUDMSFunctionLibrary.h"
#include "GameInstance/RSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/RSGamePlayerController.h"

void UHUDWidgetBase::Print_Menu(ELogMS TypeVerb, FString Str, int Line, const char* Function) const
{
    UMSFunctionLibrary::Print_Log(TypeVerb, FString::Printf(TEXT("Name widget: %s | "), *GetName()) + Str, Line, Function);
}

void UHUDWidgetBase::ShowAnim(UWidgetAnimation* Anim)
{
    SetupStateButton(EMSStateObject::Inactive);
    LOGMENU(ELogMS::Display, FString::Printf(TEXT("Show animation: %s"), *Anim->GetName()));
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
        
        LOGMENU(ELogMS::Display, FString::Printf(TEXT("Restart button state via: %f"), Anim->GetEndTime()));
    }
}

void UHUDWidgetBase::ShowAnimTimer(UWidgetAnimation* Anim, float RateTime)
{
    if (GetWorld()->GetTimerManager().TimerExists(this->TimerHandleShowAnim))
    {
        LOGMENU(ELogMS::Warning, FString::Printf(TEXT("Timer Handle: %s is exist"), *this->TimerHandleShowAnim.ToString()));
        return;
    }

    if (RateTime <= 0.0f)
    {
        LOGMENU(ELogMS::Warning, FString::Printf(TEXT("Call function with rate timer: %f <= 0.0"), RateTime));
        return;
    }
    
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UHUDWidgetBase::ShowAnim, Anim);
    GetWorld()->GetTimerManager().SetTimer(this->TimerHandleShowAnim, TimerDelegate, RateTime, false);

    LOGMENU(ELogMS::Display, FString::Printf(TEXT("Show Animation: %s via: %f"), *Anim->GetName(), Anim->GetEndTime()));
}

void UHUDWidgetBase::SetupStateButton(EMSStateObject NewState)
{
    if (this->StateWidget == EMSStateObject::Inactive) return;
    LOGMENU(ELogMS::Display, FString::Printf(TEXT("New State button: %s"), *UEnum::GetValueAsString(NewState)));
    this->StateButton = NewState;
}

void UHUDWidgetBase::SetupStateButtonTimer(EMSStateObject NewState, float RateTime)
{
    if (GetWorld()->GetTimerManager().TimerExists(this->TimerHandleButton))
    {
        LOGMENU(ELogMS::Warning, FString::Printf(TEXT("Timer Handle: %s is exist"), *this->TimerHandleButton.ToString()));
        return;
    }

    if (RateTime <= 0.0f)
    {
        LOGMENU(ELogMS::Warning, FString::Printf(TEXT("Call function with rate timer: %f <= 0.0"), RateTime));
        return;
    }

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UHUDWidgetBase::SetupStateButton, NewState);
    GetWorld()->GetTimerManager().SetTimer(this->TimerHandleShowAnim, TimerDelegate, RateTime, false);
}

void UHUDWidgetBase::SetupStateWidget(const EMSStateObject NewState)
{
    LOGMENU(ELogMS::Display, FString::Printf(TEXT("New State widget: %s"), *UEnum::GetValueAsString(NewState)));
    this->StateWidget = NewState;
    SetupStateButton(NewState);
}

void UHUDWidgetBase::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    LOGMENU(ELogMS::Display, "Native On Initialized");

    this->GameInst = UMSGameInstance::Get(GetWorld());
    if (!CHECKMS(this->GameInst != nullptr, "Game instance is nullptr")) return;

    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    GamePlayerController = Cast<ARSGamePlayerController>(PlayerController);

    GameMode = Cast<ARSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    
}
