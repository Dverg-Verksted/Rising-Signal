// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/WeaponSystem/RSBaseWeapon.h"
#include "RSRangedWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta=(EditCondition = "!Infinite"))
    int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

UCLASS()
class RISINGSIGNAL_API ARSRangedWeapon : public ARSBaseWeapon
{
	GENERATED_BODY()

public:
    FOnClipEmptySignature FOnClipEmpty;

    virtual void StartAiming();
    virtual void StopAiming();
    
    virtual void StartAttack();
    virtual void StopAttack();

    void ChangeClip();
    bool CanReload() const;

protected:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15,10,false};

    virtual void BeginPlay() override;
    virtual void MakeShot();
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

    void DecreaseAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;
    void LogAmmo();

private:
    FAmmoData CurrentAmmo;
};
