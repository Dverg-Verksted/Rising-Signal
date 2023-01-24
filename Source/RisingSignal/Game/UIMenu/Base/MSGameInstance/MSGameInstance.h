/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MSGameInstance.generated.h"

/**
 * @class The main class for storing intermediate data between levels
 */
UCLASS()
class RISINGSIGNAL_API UMSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

    // Singleton
    UFUNCTION(BlueprintCallable, Category = "UJamMSGameInstance|Singleton")
    static UMSGameInstance* Get(UWorld* World) { return (World) ? Cast<UMSGameInstance>(World->GetGameInstance()) : nullptr; }
    
#pragma region GetData
    
    /**
     * @public Getting name menu level
     * @return FName
     **/
    UFUNCTION(BlueprintPure, Category = "UMSGameInstance|GetData")
    FORCEINLINE FName GetNameMenuLevel() const { return this->NameMenuLevel; }

    /**
     * @public Getting name start level
     * @return FName
     **/
    UFUNCTION(BlueprintPure, Category = "UMSGameInstance|GetData")
    FORCEINLINE FName GetNameStartLevel() const { return this->NameStartLevel; }

    /**
     * @public Getting Music Menu Class
     * @return USoundClass*
     **/
    UFUNCTION(BlueprintPure, Category = "UMSGameInstance|GetData")
    FORCEINLINE USoundClass* GetMusicMenuClass() const { return this->MusicMenuClass; }

    /**
     * @public Getting Sound Menu Class
     * @return USoundClass*
     **/
    UFUNCTION(BlueprintPure, Category = "UMSGameInstance|GetData")
    FORCEINLINE USoundClass* GetSoundMenuClass() const { return this->SoundMenuClass; }

    /**
     * @public Getting Default Music Volume Value
     * @return float
     **/
    UFUNCTION(BlueprintPure, Category = "UMSGameInstance|GetData")
    FORCEINLINE float GetDefaultMusicVolumeValue() const { return this->DefaultMusicVolumeValue; }

    /**
     * @public Getting Default Sound Volume Value
     * @return float
     **/
    UFUNCTION(BlueprintPure, Category = "UMSGameInstance|GetData")
    FORCEINLINE float GetDefaultSoundVolumeValue() const { return this->DefaultSoundVolumeValue; }

    /**
    * @public Getting array screen size
    * @return TArray<FIntPoint>
    **/
    UFUNCTION(BlueprintPure, Category = "UMSGameInstance|GetData")
    FORCEINLINE TArray<FIntPoint> GetArrayWindowedScreenSize() const { return this->ArrayWindowedScreenSize; }

    /**
    * @public Getting language game
    * @return FString
    **/
    UFUNCTION(BlueprintPure, Category = "UMSGameInstance|GetData")
    FORCEINLINE FString GetLangGame() const { return this->LangGame; }

#pragma endregion

#pragma region SetData

    /**
    * @public Setting language game
    * @param1 FString
    **/
    UFUNCTION(BlueprintCallable, Category = "UMSGameInstance|SetData")
    void SetupLangGame(const FString NewLang);

#pragma endregion

protected:
    /** Called when the game instance is started either normally or through PIE. */
    virtual void OnStart() override;
    /** virtual function to allow custom GameInstances an opportunity to set up what it needs */
    virtual void Init() override;
    
private:
    /* level names for interaction from other classes */

    // Level start name
    UPROPERTY(EditDefaultsOnly, Category = "Levels Editor")
    FName NameStartLevel = NAME_None;

    // Level menu name
    UPROPERTY(EditDefaultsOnly, Category = "Levels Editor")
    FName NameMenuLevel = NAME_None;

    /* Volume editor for sound classes */

    // Music menu class
    UPROPERTY(EditDefaultsOnly, Category = "Volume Editor")
    USoundClass* MusicMenuClass;

    // Default value music volume
    UPROPERTY(EditDefaultsOnly, Category = "Volume Editor", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float DefaultMusicVolumeValue = 50.f;

    // Sound menu class
    UPROPERTY(EditDefaultsOnly, Category = "Volume Editor")
    USoundClass* SoundMenuClass;

    // Default value sound volume
    UPROPERTY(EditDefaultsOnly, Category = "Volume Editor", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float DefaultSoundVolumeValue = 50.f;
    
    // Array of available screen resolutions
    TArray <FIntPoint> ArrayWindowedScreenSize;

    // Current language game
    FString LangGame;
};
