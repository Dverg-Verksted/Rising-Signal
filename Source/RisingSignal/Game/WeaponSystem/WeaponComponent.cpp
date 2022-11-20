// It is owned by the company Dverg Verksted.


#include "Game/WeaponSystem/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "RSBaseWeapon.h"


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::StartAiming()
{
    
}

void UWeaponComponent::StopAiming()
{
}

void UWeaponComponent::Attack()
{
    CurrentWeapon->StartAttack();
}

void UWeaponComponent::StopAttack()
{
}

void UWeaponComponent::NextWeapon()
{
}

void UWeaponComponent::Reload()
{
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
    
    SpawnWeapons();
}

void UWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;

    CurrentWeapon = GetWorld()->SpawnActor<ARSBaseWeapon>(WeaponClass);
    
    if (!CurrentWeapon) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,false);
    
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponEquipSocketName);
    CurrentWeapon->SetOwner(Character);
}

void UWeaponComponent::SpawnWeapons(ARSBaseWeapon* BaseWeapon)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;

    BaseWeapon = GetWorld()->SpawnActor<ARSBaseWeapon>(WeaponClass);
    if (!BaseWeapon) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,false);
    BaseWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponEquipSocketName);
    BaseWeapon->SetOwner(Character);
}


void UWeaponComponent::AttachWeaponToSocket(ARSBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
}

bool UWeaponComponent::CanAim() const
{
    return true;
}

bool UWeaponComponent::CanFire() const
{
    return true;
}

bool UWeaponComponent::CanMeleeAttack() const
{
    return true;
}

bool UWeaponComponent::CanEquip() const
{
    return true; // TODO
}

bool UWeaponComponent::CanReload() const
{
    return true; // TODO
}

void UWeaponComponent::OnEmptyClip()
{
    ChangeClip();
}

void UWeaponComponent::ChangeClip()
{
    if (!CanReload()) return;
}



