// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/AbilitySystem/BaseComponents/RSAbilitySystem.h"
#include "GameFramework/Actor.h"
#include "RSAbilityInteractVolume.generated.h"

class USceneComponent;
class USphereComponent;
class UBoxComponent;
class UShapeComponent;

UCLASS(HideCategories = ("Rendering", "Replication", "Collision", "Input", "Actor",
    "LOD", "Cooking"))
class RISINGSIGNAL_API ARSAbilityInteractVolume : public AActor
{
    GENERATED_BODY()

public:
    ARSAbilityInteractVolume();

    // Component for base
    UPROPERTY()
    USceneComponent* SceneComponent;

    // Component for sphere collision
    UPROPERTY()
    USphereComponent* SphereComponent;

    // Component for box collision
    UPROPERTY()
    UBoxComponent* BoxComponent;
    
    // Shape radius
    UPROPERTY(EditAnywhere, Category = "Shape Component", DisplayName = "Размер коллизии",
        meta=(ToolTip="Если сфера, то это значение будет радиусов, если коробка, то это половина грани и тд"))
    float SphereRadius = 100.0f;

    // that value determine SPHERE collision shape
    UPROPERTY(EditAnywhere, Category = "Shape Component", DisplayName = "Форма сферы",
                meta=(ToolTip=""))
    bool IsSphereForm = false;

    // that value determine BOX collision shape
    UPROPERTY(EditAnywhere, Category = "Shape Component", DisplayName = "Форма Коробки",
                meta=(ToolTip=""))
    bool IsBoxForm = true;

    
    // Type ability system state, which will be changed when actor overlap
    UPROPERTY(EditAnywhere, Category = "Params", DisplayName = "Тип параметра",
        meta = (ToolTip="Параметр, который будет изменяться у вошедшего"))
    EAbilityStatesType AbilityStateType = EAbilityStatesType::Temp;

    // That Value was added plus to current value in ability system
    UPROPERTY(EditAnywhere, Category = "Params", DisplayName="Прибавить к параметру",
        meta = (ToolTip = "На примере температуры = Положительное значение - холод, отрицательное на согревание"))
    float AddValueToState = 0.0f;

    // That value control view actor in game
    UPROPERTY(EditAnywhere, Category = "Params", DisplayName = "Показывать в мире",
            meta=(ToolTip="При 'ложь' коллизия объекта будет показана в мире"))
    bool IsHiddenInGame = true;

    
protected:
    virtual void BeginPlay() override;

#if UE_EDITOR

    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif
    
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
    void SetStateChangedValue(const AActor* Actor, const float AddValue) const;

    void CheckIfCharactersInsideVolume();
    
};
