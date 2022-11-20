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
    if (!CurrentAmmo.Infinite)
    {
        if (CurrentAmmo.Clips==0)
        {
            LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("No more clips")));
            return;
        }
        CurrentAmmo.Clips--;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("----- Change Clip -----")));
}

bool ARSRangedWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void ARSRangedWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponMesh);
    CurrentAmmo = DefaultAmmo;
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
    if (CurrentAmmo.Bullets==0)
    {
        LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("Clip is empty")));
        return;
    }
    
    CurrentAmmo.Bullets--;
    LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopAttack();
        FOnClipEmpty.Broadcast();
    }
}

bool ARSRangedWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ARSRangedWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void ARSRangedWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    LOG_RS(ELogRSVerb::Error, FString::Printf(TEXT("%s"), *AmmoInfo));
}
