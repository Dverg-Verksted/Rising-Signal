// It is owned by the company Dverg Verksted.


#include "Game/WeaponSystem/Ranged/RSRangedWeapon.h"

#include "Library/RSFunctionLibrary.h"

void ARSRangedWeapon::StartAiming()
{
    //TODO
}

void ARSRangedWeapon::StopAiming()
{
    //TODO
}

void ARSRangedWeapon::StartAttack(){}

void ARSRangedWeapon::StopAttack(){}

void ARSRangedWeapon::ChangeClip()
{
}

bool ARSRangedWeapon::CanReload() const
{
    return true;
}

void ARSRangedWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponMesh);

}

void ARSRangedWeapon::MakeShot(){}

bool ARSRangedWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ARSRangedWeapon::DecreaseAmmo()
{

}

bool ARSRangedWeapon::IsAmmoEmpty() const
{
    return false;
}

bool ARSRangedWeapon::IsClipEmpty() const
{
    return false;
}

void ARSRangedWeapon::LogAmmo()
{
    
}
