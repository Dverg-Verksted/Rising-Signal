// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/WeaponSystem/RSBaseWeapon.h"
#include "RSMeleeWeapon.generated.h"

UCLASS()
class RISINGSIGNAL_API ARSMeleeWeapon : public ARSBaseWeapon
{
	GENERATED_BODY()

    virtual void StartAttack();
    virtual void StopAttack();

protected:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenSwing = 2.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 25.f;

    virtual void MakePunch();
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    
private:
    void MakeDamage(const FHitResult& HitResult);

    FTimerHandle MeleeTimerHandle;
};
