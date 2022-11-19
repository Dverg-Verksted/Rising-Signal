// It is owned by the company Dverg Verksted.


#include "Game/WeaponSystem/Melee/RSMeleeWeapon.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Library/RSFunctionLibrary.h"

void ARSMeleeWeapon::StartAttack()
{
    LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("Attack!!")));

    // GetWorldTimerManager().SetTimer(MeleeTimerHandle, this, &ARSMeleeWeapon::MakePunch, TimeBetweenSwing, false);
    
    MakePunch();
    PlayAnimMontage(AttackAnimMontage);
}

void ARSMeleeWeapon::StopAttack()
{
    GetWorldTimerManager().ClearTimer(MeleeTimerHandle);
}

void ARSMeleeWeapon::MakePunch()
{
    LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("You have clicked left mouse button")));

    if (!GetWorld())
    {
        LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("There is no world")));

        StopAttack();
        return;
    }
    
    FVector TraceStart, TraceEnd, HitDirection;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("There is no TraceData")));
    
        StopAttack();
        return;
    }
    
    const FTransform OwnerTransform = GetOwner()->GetTransform();
    TraceStart = OwnerTransform.GetLocation();
    HitDirection = OwnerTransform.GetRotation().GetForwardVector();
    TraceEnd = TraceStart + HitDirection * TraceMaxDistance;
    DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 1.0f,0, 3.0f);

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Emerald, false, 2.f, 0, 2.f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 2.f, 6, FColor::Red, false, 2.f);

        LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("hit was made")));
    }
    else
    {
        LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("missed one or no collision")));
    
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Yellow, false, 2.f, 0, 2.f);
    }
}

bool ARSMeleeWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("no view point")));
        return false;
    }

    TraceStart = ViewLocation;
    const FVector HitDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + HitDirection * TraceMaxDistance;
    LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("here all the trace")));
    return true;
}

void ARSMeleeWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor)
        return;

    DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
    LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("MakeDamage was here")));

}

void ARSMeleeWeapon::PlayAnimMontage(UAnimMontage* Animation)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;

    Character->PlayAnimMontage(Animation);
    LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("Animation Played")));
}
