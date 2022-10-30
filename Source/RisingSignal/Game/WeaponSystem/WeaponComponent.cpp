// It is owned by the company Dverg Verksted.


#include "Game/WeaponSystem/WeaponComponent.h"
#include "Ranged/RSRangedWeapon.h"
#include "GameFramework/Character.h"
#include "Melee/RSMeleeWeapon.h"
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

void UWeaponComponent::StartAttack()
{
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
}

void UWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;

    for ( auto OneWeaponData :  WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<ARSBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        if (Cast<ARSRangedWeapon>(Weapon))
        {
            Cast<ARSRangedWeapon>(Weapon)->FOnClipEmpty.AddUObject(this, &UWeaponComponent::OnEmptyClip);
        }        
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void UWeaponComponent::AttachWeaponToSocket(ARSBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
}

bool UWeaponComponent::CanAim() const
{
    return CurrentWeapon && Cast<ARSRangedWeapon>(CurrentWeapon);
}

bool UWeaponComponent::CanFire() const
{
    if (Cast<ARSRangedWeapon>(CurrentWeapon))
    {
        return true;
    }
    return false;
}

bool UWeaponComponent::CanMeleeAttack() const
{
    if (Cast<ARSMeleeWeapon>(CurrentWeapon))
    {
        return true;
    }
    return false;
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
    if (!Cast<ARSRangedWeapon>(CurrentWeapon)) return;
    Cast<ARSRangedWeapon>(CurrentWeapon)->StopAttack();
    Cast<ARSRangedWeapon>(CurrentWeapon)->ChangeClip();
}



