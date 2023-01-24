/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#include "Game/UIMenu/UI/MSSettingsUserWidget.h"
#include "Game/UIMenu/Base/MSGameInstance/MSGameInstance.h"
#include "Game/UIMenu/Base/MSGameMode/MSGameMode.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UMSSettingsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->BackButton->OnClicked.AddDynamic(this, &UMSSettingsUserWidget::ChangeToMainMenu);
   
    this->MusicCheckBox->CheckedState = GetGameInst()->GetMusicMenuClass()->Properties.Volume == 0.0f ? ECheckBoxState::Unchecked : ECheckBoxState::Checked;
    this->SoundCheckBox->CheckedState = GetGameInst()->GetSoundMenuClass()->Properties.Volume == 0.0f ? ECheckBoxState::Unchecked : ECheckBoxState::Checked;
    
    this->MusicCheckBox->OnCheckStateChanged.AddDynamic(this, &UMSSettingsUserWidget::ChangeMusicVolume);
    this->SoundCheckBox->OnCheckStateChanged.AddDynamic(this, &UMSSettingsUserWidget::ChangeSoundVolume);

    this->ResolutionComboBoxString->ClearOptions();
    TArray<FIntPoint> ArraySizePoint = GetGameInst()->GetArrayWindowedScreenSize();
    for (const auto Point : ArraySizePoint)
    {
        this->ResolutionComboBoxString->AddOption(FString::FromInt(Point.X) + "x" + FString::FromInt(Point.Y));
    }

    const UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
    if (!GameUserSettings)
    {
        LOGMENU(ELogVerb::Error, FString("Game user settings is nullptr"));
        return;
    }

    this->ResolutionComboBoxString->SetSelectedOption(UMSFunctionLibrary::GetStringSizeScreen(GameUserSettings->GetScreenResolution()));
    this->ResolutionComboBoxString->OnSelectionChanged.AddDynamic(this, &UMSSettingsUserWidget::ChangeSelectedScreenSize);

    this->LanguageComboBoxString->ClearOptions();
    this->LanguageComboBoxString->AddOption(FString("English"));
    this->LanguageComboBoxString->AddOption(FString("Russian"));
    this->LanguageComboBoxString->SetSelectedOption(GetGameInst()->GetLangGame() == "ru" ? "Russian" : "English");
    this->LanguageComboBoxString->OnSelectionChanged.AddDynamic(this, &UMSSettingsUserWidget::ChangeLangGame);

}

void UMSSettingsUserWidget::ChangeToMainMenu()
{
    if (GetStateButton() == EMSStateObject::Inactive) return;
    ShowAnim(BackButtonAnim);
    GetGameMode()->ChangeMenuStateTimer(EMSMenuState::MainMenu, BackButtonAnim->GetEndTime());
}

void UMSSettingsUserWidget::ChangeMusicVolume(bool bIsChecked)
{
    const float NewVolumeValue = bIsChecked ? GetGameInst()->GetDefaultMusicVolumeValue() / VALUE_PERCENT : 0.0f;
    LOGMENU(ELogVerb::Display, FString::Printf(TEXT("Name checkbox: %s | State: %s | Current Music volume: %f | New Music volume: %f"),
        *this->MusicCheckBox->GetName(), *UMSFunctionLibrary::GetStringFromBool(bIsChecked),
        GetGameInst()->GetMusicMenuClass()->Properties.Volume, NewVolumeValue));
    
    GetGameInst()->GetMusicMenuClass()->Properties.Volume = NewVolumeValue;
}

void UMSSettingsUserWidget::ChangeSoundVolume(bool bIsChecked)
{
    const float NewVolumeValue = bIsChecked ? GetGameInst()->GetDefaultSoundVolumeValue() / VALUE_PERCENT : 0.0f;
    LOGMENU(ELogVerb::Display, FString::Printf(TEXT("Name checkbox: %s | State: %s | Current Sound volume: %f | New Sound volume: %f"),
        *this->MusicCheckBox->GetName(), *UMSFunctionLibrary::GetStringFromBool(bIsChecked),
        GetGameInst()->GetSoundMenuClass()->Properties.Volume, NewVolumeValue));
    
    GetGameInst()->GetSoundMenuClass()->Properties.Volume = bIsChecked ? GetGameInst()->GetDefaultSoundVolumeValue() / VALUE_PERCENT : 0.0f;
}

void UMSSettingsUserWidget::ChangeSelectedScreenSize(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    LOGMENU(ELogVerb::Display, FString::Printf(TEXT("Selected item: %s | Selection type: %s"), *SelectedItem, *UEnum::GetValueAsString(SelectionType)));
    if (SelectionType == ESelectInfo::OnMouseClick)
    {
        UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
        if (!GameUserSettings)
        {
            LOGMENU(ELogVerb::Error, FString("Game user settings is nullptr"));
            return;
        }
        const FIntPoint Point = UMSFunctionLibrary::GetSizeScreenFromString(SelectedItem);
        if (Point.X == 0 || Point.Y == 0)
        {
            LOGMENU(ELogVerb::Error, FString::Printf(TEXT("Incorrect value screen size: X - %i | Y - %i"), Point.X, Point.Y));
            return;
        }
        GameUserSettings->SetScreenResolution(Point);
        GameUserSettings->ApplySettings(false);
    }
}

void UMSSettingsUserWidget::ChangeLangGame(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    LOGMENU(ELogVerb::Display, FString::Printf(TEXT("Selected item: %s | Selection type: %s"), *SelectedItem, *UEnum::GetValueAsString(SelectionType)));
    if (SelectionType == ESelectInfo::OnMouseClick)
    {
        GetGameInst()->SetupLangGame((SelectedItem == "English") ? "en" : "ru");
    }
}
