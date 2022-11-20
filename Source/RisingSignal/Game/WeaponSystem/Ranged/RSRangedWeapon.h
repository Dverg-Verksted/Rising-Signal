// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/WeaponSystem/RSBaseWeapon.h"
#include "RSRangedWeapon.generated.h"

UCLASS()
class RISINGSIGNAL_API ARSRangedWeapon : public ARSBaseWeapon
{
	GENERATED_BODY()

public:
    virtual void StartAiming();
    virtual void StopAiming();
    virtual void StartAttack();
    virtual void StopAttack();
    
    void ChangeClip();
    bool CanReload() const;

protected:
    virtual void BeginPlay() override;
    virtual void MakeShot();
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

    void DecreaseAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;
    void LogAmmo();
};
