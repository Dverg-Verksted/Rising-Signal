// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class RISINGSIGNAL_API ARSBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ARSBaseWeapon();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

	virtual void BeginPlay() override;

    APlayerController* GetPlayerController() const;
    FVector GetMuzzleWorldLocation() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
};
