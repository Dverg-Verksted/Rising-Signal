/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#include "Game/UIMenu/MSHUD.h"
#include "Game/UIMenu/Base/HUDGameMode/MSGameMode.h"
#include "Game/UIMenu/Library/HUDMSFunctionLibrary.h"
#include "Game/UIMenu/UI/MSMenuUserWidgetBase.h"

void AMSHUD::BeginPlay()
{
    Super::BeginPlay();

    this->GameMode = AMSGameMode::Get(GetWorld());
    if (!CHECK(this->GameMode != nullptr, "Game mode is nullptr")) return;

    // if (!CHECK(this->WelcomeHudWidgetClass.GetDefaultObject() != nullptr, "WelcomeHudWidgetClass is nullptr")) return;
    // if (!CHECK(this->MenuHudWidgetClass.GetDefaultObject() != nullptr, "MenuHudWidgetClass is nullptr")) return;
    // if (!CHECK(this->SettingsHudWidgetClass.GetDefaultObject() != nullptr, "OptionsHudWidgetClass is nullptr")) return;
    // if (!CHECK(this->CreditsHudWidgetClass.GetDefaultObject() != nullptr, "CreditsHudWidgetClass is nullptr")) return;

    this->MenuWidgets.Add(EMSMenuState::WelcomeToGame,
        CreateWidget<UMSMenuUserWidgetBase>(GetWorld(), this->WelcomeHudWidgetClass));
    this->MenuWidgets.Add(EMSMenuState::MainMenu,
        CreateWidget<UMSMenuUserWidgetBase>(GetWorld(), this->MenuHudWidgetClass));
    this->MenuWidgets.Add(EMSMenuState::Settings,
        CreateWidget<UMSMenuUserWidgetBase>(GetWorld(), this->SettingsHudWidgetClass));
    this->MenuWidgets.Add(EMSMenuState::Credits,
        CreateWidget<UMSMenuUserWidgetBase>(GetWorld(), this->CreditsHudWidgetClass));
    this->MenuWidgets.Add(EMSMenuState::Loading,
        CreateWidget<UMSMenuUserWidgetBase>(GetWorld(), this->LoadingHudWidgetClass));

    for (const auto Widget : this->MenuWidgets)
    {
        const auto ValueWidget = Widget.Value;
        ValueWidget->AddToViewport();
        ValueWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    
    this->GameMode->OnMenuStateChanged.AddDynamic(this, &AMSHUD::OnMenuStateChanged);
}

void AMSHUD::OnMenuStateChanged(EMSMenuState NewState)
{
    // LOGJAM(ELogVerb::Display, FString::Printf(TEXT("New State in HUD: %s"), *UEnum::GetValueAsString(NewState)));

    // if (!CHECK(this->MenuWidgets.Contains(NewState), FString::Printf(TEXT("State: %s don't contains in TMap widgets"),
    //     *UEnum::GetValueAsString(NewState)))) return;
    
    if (this->MenuWidget)
    {
        FTimerHandle TimerHandle;
        FTimerDelegate TimerDelegate;
        this->MenuWidget->SetupStateWidget(EMSStateObject::Inactive);
        this->MenuWidget->ShowAnim(this->MenuWidget->GetEndAnim());

        TimerDelegate.BindUObject(this, &AMSHUD::SwitchWidget, this->MenuWidget, this->MenuWidgets[NewState]);
        GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, this->MenuWidget->GetEndAnim()->GetEndTime(), false);
    }
    else
    {
        this->SetupNewWidget(this->MenuWidgets[NewState]);
    }
}

void AMSHUD::SetupNewWidget(UMSMenuUserWidgetBase* Widget)
{
    if (!CHECK(Widget != nullptr, "Widget is nullptr")) return;
    if (this->MenuWidget == Widget)
    {
        // LOGJAM(ELogVerb::Warning, FString::Printf(TEXT("Menu widget equal pointer on new widget: %s"), *Widget->GetName()));
        return;
    }
    
    this->MenuWidget = Widget;
    this->MenuWidget->SetupStateWidget(EMSStateObject::Active);
    this->MenuWidget->SetVisibility(ESlateVisibility::Visible);
    if (this->GameMode->GetMenuState() != EMSMenuState::Loading)
    {
        this->MenuWidget->ShowAnim(this->MenuWidget->GetStartAnim());
    }
}

void AMSHUD::SwitchWidget(UMSMenuUserWidgetBase* OldWidget, UMSMenuUserWidgetBase* NewWidget)
{
    OldWidget->SetVisibility(ESlateVisibility::Hidden);
    this->MenuWidget = nullptr;
    this->SetupNewWidget(NewWidget);
}
