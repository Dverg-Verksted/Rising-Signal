// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Game/InteractSystem/Environment/InteractiveActor.h"
#include "ClimbingWall.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API AClimbingWall : public AInteractiveActor
{
	GENERATED_BODY()

public:

    AClimbingWall();

    virtual void OnConstruction(const FTransform& Transform) override;

    float GetWallLength() const { return WallLength; }

    float GetWallWidth() const { return WallWidth; }

    float GetInterpSpeed() const { return InterpSpeed; }

protected:

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wall | Parameters", DisplayName="Высота стены")
    float WallLength = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wall | Parameters", DisplayName="Ширина стены")
    float WallWidth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wall | Parameters", DisplayName="Скорость интерполяции",
    meta=(ToolTip="Скорость с которой будет интерполироваться поворот персонажа, при переходе на эту стенку"))
    float InterpSpeed = 2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* Wall;

    UBoxComponent* GetInteractionBox() {return StaticCast<UBoxComponent*>(InteractionVolume); }

    virtual void OnInteractionVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
    virtual void OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
