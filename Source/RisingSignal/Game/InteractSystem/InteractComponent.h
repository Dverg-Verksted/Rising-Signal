// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

class UBoxComponent;
class ARSGamePLayer;

/**
 * @class
 **/
UCLASS(ClassGroup = (Managers), meta = (BlueprintSpawnableComponent),
	HideCategories = ("Variable", "Transform", "Sockets", "Shape", "Navigation", "ComponentTick", "Physics", "Tags", "Cooking", "HLOD",
		"Mobile", "Activation", "Component Replication", "Events", "Asset User Data", "Collision"))
class RISINGSIGNAL_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#if WITH_EDITOR

	/**
	 * @protected Called whenever a variable changes
	 **/
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

private:
	// @private Size box collision
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Interact", meta = (AllowPrivateAccess = true))
	FVector SizeBoxCollision = FVector(50.f);

	// @private Owner component
	UPROPERTY()
	ARSGamePLayer* OwnerPlayer;

	// @private Box component Collision
	UPROPERTY()
	UBoxComponent* BoxCollision;
};
