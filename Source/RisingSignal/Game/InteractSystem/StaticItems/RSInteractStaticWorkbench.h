// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/InteractSystem/RSInteractStaticItemBase.h"
#include "RSInteractStaticWorkbench.generated.h"

class AInteractItemActor;
class USphereComponent;
/**
 * 
 */
UCLASS(HideCategories = ("Variable", "Sockets", "Shape", "Navigation", "ComponentTick", "Physics", "Tags", "Cooking", "HLOD",
    "Mobile", "Activation", "Component Replication", "Events", "Asset User Data", "Collision", "Rendering", "Input", "Actor", "LOD"))
class RISINGSIGNAL_API ARSInteractStaticWorkbench : public ARSInteractStaticItemBase
{
    GENERATED_BODY()

public:
    virtual void Interact(ACharacter* InteractingCharacter) override;

protected:
    ARSInteractStaticWorkbench();

    virtual void BeginPlay() override;

    UFUNCTION()
    void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    /**
     * Increase or decrease character temperature based on bHeatUp value
     * @param Character Character to whom change temperature
     * @param bCharInside marks that character inside or not
     */
    void CharacterInsideVolume(ACharacter* Character, const bool bCharInside);

    void CheckIfCharactersInsideVolume();

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* BaseMesh;

    UPROPERTY()
    AInteractItemActor* ParentInteractActor;

private:
};
