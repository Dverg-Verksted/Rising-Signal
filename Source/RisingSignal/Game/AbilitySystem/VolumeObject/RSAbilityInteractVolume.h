﻿// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/AbilitySystem/BaseComponents/RSAbilitySystem.h"
#include "GameFramework/Actor.h"
#include "RSAbilityInteractVolume.generated.h"

class USphereComponent;
class UBoxComponent;

UCLASS(HideCategories = ("Rendering", "Replication", "Collision", "Input", "Actor",
    "LOD", "Cooking"))
class RISINGSIGNAL_API ARSAbilityInteractVolume : public AActor
{
    GENERATED_BODY()

public:
    ARSAbilityInteractVolume();

    // Trying to do changing collision shape
    UPROPERTY(EditAnywhere, Category = "Trigger Component", DisplayName = "Форма коллизии")
    USphereComponent* SphereComponent;

    // Shape radius
    UPROPERTY(EditAnywhere, Category = "Params", DisplayName = "Размер коллизии",
        meta=(ToolTip="Если сфера, то это значение будет радиусов, если коробка, то это половина грани и тд"))
    float SphereRadius = 100.0f;

    // Type ability system state, which will be changed when actor overlap
    UPROPERTY(EditAnywhere, Category = "Params", DisplayName = "Тип параметра",
        meta = (ToolTip="Параметр, который будет изменяться у вошедшего"))
    EAbilityStatesType AbilityStateType = EAbilityStatesType::Temp;

    // That Value was added plus to current value in ability system
    UPROPERTY(EditAnywhere, Category = "Params", DisplayName="Прибавить к параметру",
        meta = (ToolTip = "На примере температуры = Положительное значение - холод, отрицательное на согревание"))
    float AddValueToState = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Params", DisplayName = "Показывать в мире",
            meta=(ToolTip="При 'ложь' коллизия объекта будет показана в мире"))
    bool IsHiddenInGame = true;
    
protected:
    virtual void BeginPlay() override;

private:
    
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult &SweepResult);
    
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, 
                      AActor* OtherActor, 
                      UPrimitiveComponent* OtherComp, 
                      int32 OtherBodyIndex);

    /**
     * @brief Setup value for change choose state in actor's ability system 
     * @param Actor - Whom from taken ability system
     * @param AbilityStType - Type ability in system, that we will changed
     * @param AddValue - Value, that will added plus to changed value in state in ability system
     */
    void SetStateChangedValue(const AActor* Actor, const EAbilityStatesType AbilityStType, const float AddValue) const;
    
};
