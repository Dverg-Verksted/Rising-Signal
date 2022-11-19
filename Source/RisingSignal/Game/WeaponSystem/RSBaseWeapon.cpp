// It is owned by the company Dverg Verksted.


#include "Game/WeaponSystem/RSBaseWeapon.h"

#include "AlsCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Library/RSFunctionLibrary.h"

ARSBaseWeapon::ARSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ARSBaseWeapon::StartAiming() {}

void ARSBaseWeapon::StopAiming() {}

void ARSBaseWeapon::StartAttack() {}

void ARSBaseWeapon::StopAttack() {}

void ARSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
    check(WeaponMesh);
}

bool ARSBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
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

APlayerController* ARSBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<AAlsCharacter>(GetOwner());
    if (!Player)
    {
        LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("no player")));
        return nullptr;
    }

    return Player->GetController<APlayerController>();
}

FVector ARSBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ARSBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if (!Controller)
    {
        LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("no controller")));
        return false;
    }

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

void ARSBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld())
        return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
    LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("Make hit was here")));
}