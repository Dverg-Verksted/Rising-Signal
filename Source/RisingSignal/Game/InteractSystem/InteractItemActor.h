// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractItemActor.generated.h"

class UInteractItemDataAsset;
UCLASS()
class RISINGSIGNAL_API AInteractItemActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AInteractItemActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

#if UE_EDITOR

    // Called when changed property
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

private:
    // @private
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    // @private Soft pointer on InteractItemDataAsset
    UPROPERTY(
        EditAnywhere, Category = "Settings Interact", meta = (DisplayName = "Данные об предмете", ToolTip = "Укажите данные об предмете"))
    TSoftObjectPtr<UInteractItemDataAsset> InteractItem;
};
