// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/AbilitySystem/BaseComponents/RSAbilitySystem.h"
#include "GameFramework/Actor.h"
#include "RSTempVolume.generated.h"

class USphereComponent;
class UBoxComponent;

UCLASS()
class RISINGSIGNAL_API ARSTempVolume : public AActor
{
    GENERATED_BODY()

public:
    ARSTempVolume();

    // Попытки сделать изменяемую коллизию
    UPROPERTY(EditAnywhere, Category = "Trigger Component", DisplayName = "Форма коллизии")
    USphereComponent* SphereComponent;

    // Радиус сферы
    UPROPERTY(EditAnywhere, Category = "Params", DisplayName = "Размер коллизии",
        meta=(ToolTip="Если сфера, то это значение будет радиусов, если коробка, то это половина грани и тд"))
    float SphereRadius = 100.0f;

    // Тип стейта из абилити системы, который будет изменяться у вошедшего в волюм актера
    UPROPERTY(EditAnywhere, Category = "Params", DisplayName = "Тип параметра абилити системы",
        meta = (ToolTip="Параметр, который будет изменяться у вошедшего"))
    EAbilityStatesType AbilityStateType = EAbilityStatesType::Temp;

    // Добавляемое значение к выбранному ранее стейту из абилити системы вошедшего актера
    UPROPERTY(EditAnywhere, Category = "Params", DisplayName="Добавить к значению на изменение у типа",
        meta = (ToolTip = "На примере температуры = Положительное значение - холод, отрицательное на согревание"))
    float AddValueToState = 0.0f;
    
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

    /** Установка значения изменения для выбранного стейта в абилити компоненте актера
     * @param Actor - тот, у кого берется абилитисистем для изменения параметров
     * @param AbilityStType - тип абилити у которого будет меняться значение
     * @param AddValue - цифра, на которую будет изменен параметр в абилити системе,
     * отвечающий за изменение стейта в секунду
     */
    void SetStateChangedValue(const AActor* Actor, const EAbilityStatesType AbilityStType, const float AddValue) const;
    
};
