/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/
#pragma once

#include "CoreMinimal.h"
#include "Game/UIMenu/Library/HUDMSFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Game/UIMenu/Base/MSDataTypes.h"
#include "MSMenuUserWidgetBase.generated.h"

// Forward declare
class UMSGameInstance;
class AMSGameMode;

#define LOGMENU(verb, str) Print_Menu(verb, str, __LINE__, __FUNCTION__)

/**
 * @class Parent class for all widget menu system
 */
UCLASS()
class RISINGSIGNAL_API UMSMenuUserWidgetBase : public UUserWidget
{
    GENERATED_BODY()

public:

    // Log print from menu
    void Print_Menu(ELogMS TypeVerb, FString Str, int Line, const char* Function) const;
    
#pragma region Action

    /**
     * @public Show animation
     * @param1 UWidgetAnimation
     **/
    UFUNCTION(BlueprintCallable, Category = "UMenuUserWidgetBase|Action")
    void ShowAnim(UWidgetAnimation* Anim);

    /**
     * @public Show animation for timer
     * @param1 UWidgetAnimation
     * @param2 float
     **/
    UFUNCTION(BlueprintCallable, Category = "UMenuUserWidgetBase|Action")
    void ShowAnimTimer(UWidgetAnimation* Anim, float RateTime);

    /**
     * @public Setup new state widget (Intended for override)
     * @param1 EStateObject
     **/
    virtual void SetupStateWidget(const EMSStateObject NewState);

#pragma endregion

#pragma region GetData

    /**
     * @public Getting Start animation
     * @return UWidgetAnimation
     **/
    UFUNCTION(BlueprintPure, Category = "UMenuUserWidgetBase|GetData")
    FORCEINLINE UWidgetAnimation* GetStartAnim() const { return this->StartAnim; }

    /**
     * @public Getting End animation
     * @return UWidgetAnimation
     **/
    UFUNCTION(BlueprintPure, Category = "UMenuUserWidgetBase|GetData")
    FORCEINLINE UWidgetAnimation* GetEndAnim() const { return this->EndAnim; }

    /**
     * @public Getting Current pointer on AJamMSGameMode class
     * @return AJamMSGameMode
     **/
    UFUNCTION(BlueprintPure, Category = "UMenuUserWidgetBase|GetData")
    FORCEINLINE AMSGameMode* GetGameMode() const { return this->GameMode; }

    /**
     * @public Getting Current pointer on UJamMSGameInstance class
     * @return UJamMSGameInstance
     **/
    UFUNCTION(BlueprintPure, Category = "UMenuUserWidgetBase|GetData")
    FORCEINLINE UMSGameInstance* GetGameInst() const { return this->GameInst; }

    /**
     * @public Getting current state widget
     * @return EStateObject
     **/
    UFUNCTION(BlueprintPure, Category = "UMenuUserWidgetBase|GetData")
    FORCEINLINE EMSStateObject GetStateWidget() const { return this->StateWidget; }

    /**
     * @public Getting current state button
     * @return EStateObject
     **/
    UFUNCTION(BlueprintPure, Category = "UMenuUserWidgetBase|GetData")
    FORCEINLINE EMSStateObject GetStateButton() const { return this->StateButton; }

#pragma endregion

protected:
    // Native handling for SObjectWidget
    virtual void NativeOnInitialized() override;

    /**
     * @public Show animation for timer
     * @param1 UWidgetAnimation
     * @param2 float
     **/
    UFUNCTION(BlueprintCallable, Category = "UMenuUserWidgetBase|Action")
    void SetupStateButton(EMSStateObject NewState);

    /**
     * @public Show animation for timer
     * @param1 UWidgetAnimation
     * @param2 float
     **/
    UFUNCTION(BlueprintCallable, Category = "UMenuUserWidgetBase|Action")
    void SetupStateButtonTimer(EMSStateObject NewState, float RateTime);

private:
    // @private Current pointer on AJamMSGameMode class
    UPROPERTY()
    AMSGameMode* GameMode;

    // @private Current pointer on UJamMSGameInstance class
    UPROPERTY()
    UMSGameInstance* GameInst;
    
    // @private State widget
    EMSStateObject StateWidget = EMSStateObject::Inactive;

    // @private State Button
    EMSStateObject StateButton = EMSStateObject::Inactive;

    // @private Animation at the start of visibility
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* StartAnim;

    // @private Animation at the end of visibility
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* EndAnim;

    // @private Timer Handle for show animation
    FTimerHandle TimerHandleShowAnim;

    // @private Timer Handle for show animation
    FTimerHandle TimerHandleButton;
};
