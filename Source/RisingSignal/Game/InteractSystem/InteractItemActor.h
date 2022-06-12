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

private:
	// @private Soft pointer on InteractItemDataAsset
	UPROPERTY(EditInstanceOnly, Category = "Settings Interact", meta = (AllowedClasses = "InteractItemDataAsset"))
	FSoftObjectPath InteractItem;
};
