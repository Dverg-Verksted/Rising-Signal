// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/UIMenu/Library/HUDMSFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Game/UIMenu/Base/MSDataTypes.h"
#include "HUDWidgetBase.generated.h"

class UMSGameInstance;
class ARSGamePlayerController;
// Forward declare
class URSGameInstance;
class ARSGameMode;

#define LOGMENU(verb, str) Print_Menu(verb, str, __LINE__, __FUNCTION__)

/**
 * @class Parent class for all hud widgets
 */
UCLASS()
class RISINGSIGNAL_API UHUDWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

    // Log print from menu
    void Print_Menu(ELogMS TypeVerb, FString Str, int Line, const char* Function) const;

#pragma region Action

    /**
     * @brief Show Animation
     * @param Anim UWidgetAnimation
     */
    UFUNCTION(BlueprintCallable, Category = "WidgetBase|Action")
    void ShowAnim(UWidgetAnimation* Anim);

    /**
     * @brief 
     * @param Anim 
     * @param RateTime 
     */
    UFUNCTION(BlueprintCallable, Category = "WidgetBase|Action")
    void ShowAnimTimer(UWidgetAnimation* Anim, float RateTime);
    
    /**
     * @brief 
     * @param NewState 
     */
    virtual void SetupStateWidget(const EMSStateObject NewState);

#pragma endregion Action

#pragma region GetData

    // UFUNCTION(BlueprintPure, Category = "WidgetBase|GetData")
    // FORCEINLINE UWidgetAnimation* GetStartAnim() const { return this->StartAnim;}
    //
    // /**
    //     * @public Getting End animation
    //     * @return UWidgetAnimation
    //     **/
    // UFUNCTION(BlueprintPure, Category = "WidgetBase|GetData")
    // FORCEINLINE UWidgetAnimation* GetEndAnim() const { return this->EndAnim; }

    /**
     * @public Getting Current pointer on AMSGameMode class
     * @return AMSGameMode
     **/
    UFUNCTION(BlueprintPure, Category = "WidgetBase|GetData")
    FORCEINLINE ARSGameMode* GetGameMode() const { return this->GameMode; }

    /**
     * @public Getting Current pointer on UMSGameInstance class
     * @return UMSGameInstance
     **/
    UFUNCTION(BlueprintPure, Category = "WidgetBase|GetData")
    FORCEINLINE UMSGameInstance* GetGameInst() const { return this->GameInst; }

    /**
     * @public Getting current state widget
     * @return EStateObject
     **/
    UFUNCTION(BlueprintPure, Category = "WidgetBase|GetData")
    FORCEINLINE EMSStateObject GetStateWidget() const { return this->StateWidget; }

    /**
     * @public Getting current state button
     * @return EStateObject
     **/
    UFUNCTION(BlueprintPure, Category = "WidgetBase|GetData")
    FORCEINLINE EMSStateObject GetStateButton() const { return this->StateButton; }

#pragma endregion GetData
    
protected:
    
    // Native handling for SObjectWidget
    virtual void NativeOnInitialized() override;

    /**
     * @public Show animation for timer
     * @param1 UWidgetAnimation
     * @param2 float
     **/
    UFUNCTION(BlueprintCallable, Category = "WidgetBase|Action")
    void SetupStateButton(EMSStateObject NewState);

    /**
     * @public Show animation for timer
     * @param1 UWidgetAnimation
     * @param2 float
     **/
    UFUNCTION(BlueprintCallable, Category = "WidgetBase|Action")
    void SetupStateButtonTimer(EMSStateObject NewState, float RateTime);
    
    UPROPERTY()
    APlayerController* PlayerController;

    UPROPERTY()
    ARSGamePlayerController* GamePlayerController;
    
private:

    // @private Current pointer on AMSGameMode class
    UPROPERTY()
    ARSGameMode* GameMode;

    // @private Current pointer on UMSGameInstance class
    UPROPERTY()
    UMSGameInstance* GameInst;
    
    // @private State widget
    EMSStateObject StateWidget = EMSStateObject::Inactive;

    // @private State Button
    EMSStateObject StateButton = EMSStateObject::Inactive;

    // @private Animation at the start of visibility
    // UPROPERTY(Transient, meta = (BindWidgetAnim))
    // UWidgetAnimation* StartAnim;
    //
    // // @private Animation at the end of visibility
    // UPROPERTY(Transient, meta = (BindWidgetAnim))
    // UWidgetAnimation* EndAnim;

    // @private Timer Handle for show animation
    FTimerHandle TimerHandleShowAnim;

    // @private Timer Handle for show animation
    FTimerHandle TimerHandleButton;

    
};
