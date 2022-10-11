// It is owned by the company Dverg Verksted.


#include "Game/WeaponSystem/Melee/RSMeleeWeapon.h"

void ARSMeleeWeapon::StartMeleeAttack()
{
}

void ARSMeleeWeapon::StopMeleeAttack()
{
}

void ARSMeleeWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ARSMeleeWeapon::MakePunch()
{
}

bool ARSMeleeWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
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
