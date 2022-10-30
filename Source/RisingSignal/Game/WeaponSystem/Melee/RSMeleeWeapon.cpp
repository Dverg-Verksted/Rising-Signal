// It is owned by the company Dverg Verksted.


#include "Game/WeaponSystem/Melee/RSMeleeWeapon.h"

#include "DrawDebugHelpers.h"

void ARSMeleeWeapon::StartAttack()
{
    GetWorldTimerManager().SetTimer(MeleeTimerHandle, this, &ARSMeleeWeapon::MakePunch, TimeBetweenSwing, false);
    MakePunch();
}

void ARSMeleeWeapon::StopAttack()
{
    GetWorldTimerManager().ClearTimer(MeleeTimerHandle);
}

void ARSMeleeWeapon::MakePunch()
{
    if (!GetWorld())
    {
        StopAttack();
        return;
    }
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopAttack();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Emerald, false, 2.f, 0, 2.f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 2.f, 6, FColor::Red, false, 2.f);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Yellow, false, 2.f, 0, 2.f);
    }
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

void ARSMeleeWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor)
        return;

    DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}
