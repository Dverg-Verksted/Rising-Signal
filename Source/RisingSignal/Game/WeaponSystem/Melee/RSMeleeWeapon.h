// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/WeaponSystem/RSBaseWeapon.h"
#include "RSMeleeWeapon.generated.h"

UCLASS()
class RISINGSIGNAL_API ARSMeleeWeapon : public ARSBaseWeapon
{
	GENERATED_BODY()

    virtual void StartMeleeAttack();
    virtual void StopMeleeAttack();

protected:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 500.0f;

    virtual void BeginPlay() override;
    virtual void MakePunch();
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    
	
};
