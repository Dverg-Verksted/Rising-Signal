// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Game/AI/RSAICharacter.h"
#include "RSAICharacter_Wolf.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API ARSAICharacter_Wolf : public ARSAICharacter
{
    GENERATED_BODY()

public:
    ARSAICharacter_Wolf();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION()
    void TimelineProgress(float Value);

    UFUNCTION(BlueprintCallable)
    float GetAlertLevel() const { return CurrentAlertLevel; }


protected:
    virtual void EnemyNoticed(bool IsNoticed) override;

    // Switching Materials from White to Black
    void SwitchWolfColorToBlack(bool bSwitch);

    // UPROPERTY(EditDefaultsOnly, Category = "FX", meta=(ToolTip = "Таймлайн изменения цвета волка"))
    FTimeline SwitchColorTimeline;

    UPROPERTY(EditDefaultsOnly, Category = "FX", meta=(ToolTip = "Кривая изменения цвета волка"))
    UCurveFloat* SwitchColorCurve;
};
