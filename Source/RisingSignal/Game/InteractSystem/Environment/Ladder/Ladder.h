// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/InteractSystem/Environment/InteractiveActor.h"
#include "Ladder.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API ALadder : public AInteractiveActor
{
	GENERATED_BODY()
    
public:
    
    ALadder();

    virtual void OnConstruction(const FTransform& Transform) override;

    virtual void BeginPlay() override;

    float GetLadderHeight() const;

    bool GetIsOnTop() const;

    UAnimMontage* GetAttachFromTopAnimMontage() const;

    FVector GetAttachFromTopAnimMontageStartingLocation() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters")
    float LadderHeight = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters")
    float LadderWeight = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters")
    float StepsInterval = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters")
    float BottomStepOffset = 25.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* RightRailMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* LeftRailMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UInstancedStaticMeshComponent* StepsMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UBoxComponent* TopInteractionVolume;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters")
    UAnimMontage* AttachFromTopAnimMontage;

    // Offset from ladder's top for starting anim montage;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters")
    FVector AttachFromTopAnimMontageInitialOffset = FVector::ZeroVector;

    virtual void OnInteractionVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    virtual void OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

    UBoxComponent* GetMeshInteractiveBox();

private:
    bool bIsOnTop = false;
};
