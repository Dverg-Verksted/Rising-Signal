// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSTempVolume.generated.h"

class USphereComponent;

UCLASS()
class RISINGSIGNAL_API ARSTempVolume : public AActor
{
    GENERATED_BODY()

public:
    ARSTempVolume();

protected:
    virtual void BeginPlay() override;

private:

    UPROPERTY(EditAnywhere, Category = "Trigger Component")
    USphereComponent* SphereComponent;

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
    
};
