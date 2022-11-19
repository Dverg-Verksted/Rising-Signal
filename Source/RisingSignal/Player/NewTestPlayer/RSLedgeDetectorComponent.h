// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSLedgeDetectorComponent.generated.h"

USTRUCT(BlueprintType)
struct FLedgeDescription
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Ledge description")
    FVector Location;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ledge description")
    FVector LedgeNormal;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ledge description")
    FRotator Rotation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ledge description")
    UPrimitiveComponent* PrimitiveComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ledge description")
    FVector PrimitiveInitialLocation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RISINGSIGNAL_API URSLedgeDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    bool LedgeDetect(OUT FLedgeDescription& LedgeDescription);

protected:
	
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ledge Detection: Settings")
    float MinimumLedgeHeight = 40.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ledge Detection: Settings")
    float MaximumLedgeHeight = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ledge Detection: Settings")
    float ForwardCheckDistance = 100.0f;

private:
    TWeakObjectPtr<ACharacter> CachedCharacterOwner;
};
