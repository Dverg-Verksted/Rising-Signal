// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

UCLASS()
class RISINGSIGNAL_API AInteractiveActor : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

    bool IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp) const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
    UPrimitiveComponent* InteractionVolume;

    UFUNCTION()
    virtual void OnInteractionVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
