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

    UPROPERTY(EditAnywhere, Category = "Trigger Component")
    USphereComponent* SphereComponent;

    UPROPERTY(EditAnywhere, Category = "Trigger Component")
    UShapeComponent* ShapeComponent;
    
    UPROPERTY(EditAnywhere, Category = "Params")
    float SphereRadius = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Params")
    EAbilityStatesType AbilityStateType = EAbilityStatesType::Temp;

    UPROPERTY(EditAnywhere, Category = "Params")
    float ChangedValueModifier = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Params")
    float BoxComponentWidth = 100.0f;
    UPROPERTY(EditAnywhere, Category = "Params")
    float BoxComponentHeight = 100.0f;
    UPROPERTY(EditAnywhere, Category = "Params")
    float BoxComponentDepth = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Params")
    bool bIsBox = false;
    
    
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

    void SetStateChangedValue(AActor* Actor, EAbilityStatesType AbilityType, float ChangedValModifier);
    
};
