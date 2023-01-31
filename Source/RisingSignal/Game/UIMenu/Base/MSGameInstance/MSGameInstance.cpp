/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/
#include "Game/UIMenu/Base/MSGameInstance/MSGameInstance.h"
#include "Game/SaveLoad/RSSaveGame.h"
#include "Game/SaveLoad/RSSaveGameSubsystem.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/UIMenu/Library/HUDMSFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Library/RSFunctionLibrary.h"

void UMSGameInstance::OnStart()
{
    Super::OnStart();
}

void UMSGameInstance::Init()
{
    Super::Init();
    NameContinueLevel = GetSavedLevelName();
    this->MusicMenuClass->Properties.Volume = this->DefaultMusicVolumeValue / VALUE_PERCENT;
    LOGMS(ELogMS::Display, FString::Printf(TEXT("Setup default music volume: %f"),
        this->MusicMenuClass->Properties.Volume));
    this->SoundMenuClass->Properties.Volume = this->DefaultSoundVolumeValue / VALUE_PERCENT;
    LOGMS(ELogMS::Display, FString::Printf(TEXT("Setup default sound volume: %f"),
        this->SoundMenuClass->Properties.Volume));
    UKismetSystemLibrary::GetConvenientWindowedResolutions(this->ArrayWindowedScreenSize);
    LOGMS(ELogMS::Display, "---| Available screen resolutions |---");
    for (const auto Point : this->ArrayWindowedScreenSize)
    {
        LOGMS(ELogMS::Display, FString::Printf(TEXT("Screen: %s"), *Point.ToString()));
    }
    if (this->ArrayWindowedScreenSize.Num() > 0)
    {
        LOGMS(ELogMS::Display, FString::Printf(TEXT("Selected default screen size: %s"),
            *UMSFunctionLibrary::GetStringSizeScreen(this->ArrayWindowedScreenSize[0])));
        UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
        if (GameUserSettings)
        {
            GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
            GameUserSettings->SetScreenResolution(this->ArrayWindowedScreenSize[0]);
            GameUserSettings->ApplySettings(false);
        }
    }
    this->LangGame = UKismetSystemLibrary::GetDefaultLanguage();
    if (!LangGame.IsEmpty())
    {
        LOGMS(ELogMS::Display, FString::Printf(TEXT("Lang game: %s"), *this->LangGame));
        this->SetupLangGame((this->LangGame != "ru" && this->LangGame != "ru-RU") ? "en" : "ru");
    }
}

FName UMSGameInstance::GetSavedLevelName()
{
    URSSaveGameSubsystem* SG = GetSubsystem<URSSaveGameSubsystem>();
    if (UGameplayStatics::DoesSaveGameExist(SG->GetSlotName(), 0))
    {
        LOG_RS(ELogRSVerb::Warning, "IF 1");
        URSSaveGame* CurrentSaveGame = Cast<URSSaveGame>(UGameplayStatics::LoadGameFromSlot(SG->GetSlotName(), 0));
        if (!CurrentSaveGame)
        {
            LOG_RS(ELogRSVerb::Warning, "IF 2");

            UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
            return NAME_None;
        }
        return CurrentSaveGame->SavedLevelName;
    }
    return NAME_None;
}

void UMSGameInstance::SetupLangGame(const FString NewLang)
{
    this->LangGame = NewLang;
    UKismetInternationalizationLibrary::SetCurrentCulture(this->LangGame);
}
