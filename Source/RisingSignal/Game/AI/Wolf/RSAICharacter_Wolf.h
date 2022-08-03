﻿// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Game/AI/RSAICharacter.h"
#include "RSAICharacter_Wolf.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS(HideCategories=("Rendering", "Collision", "Actor", "Input", "LOD", "Cooking", "Replication", "Actor Tick"),
    AutoCollapseCategories=("Character", "Camera", "Pawn"))
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

    virtual void AIStateChanged(EAIState NewState, EAIState PrevState) override;

    virtual void Attack(AActor* AttackActor) override;

    virtual void ProvideDamage(USkeletalMeshComponent* FromMeshComponent) override;


protected:
    // Switching Materials from White to Black
    void SwitchWolfColorToBlack(bool bSwitch);

    FTimeline SwitchColorTimeline;

    UPROPERTY(EditDefaultsOnly, Category = "Wolf | Furr | FX", meta=(ToolTip = "Кривая изменения цвета волка"))
    UCurveFloat* SwitchColorCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wolf | Attack | Base Config",
        meta = (ToolTip ="Дистанция, на которой игрок получит урон. Отсчитывается от челюсти"))
    float AttackDistance = 10.0;

    UPROPERTY(EditAnywhere, Category = "Wolf | Attack | Animations", meta=(ToolTip="АнимМонтаж укуса"))
    UAnimMontage* BiteAnimMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wolf | Attack | FX",
        meta = (ToolTip = "Эффект выплеска крови. Вылетает из игрока"))
    UNiagaraSystem* BloodEffect;

    void InitAnimations();
};
