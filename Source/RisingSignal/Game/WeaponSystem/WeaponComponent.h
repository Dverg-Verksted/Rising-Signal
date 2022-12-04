// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/Inventory/InventoryTypes.h"
#include "WeaponComponent.generated.h"

class ARSBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RISINGSIGNAL_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

    void StartAiming();
    void StopAiming();
    void Attack();
    void HeavyAttack();
    void StopAttack();
    
    void NextWeapon();
    void Reload();

    void SpawnWeapons();
    void SpawnWeapons(ARSBaseWeapon* BaseWeapon);

    FWeaponSettings GetWeaponSettings() const {return WeaponSettings;}

protected:
	virtual void BeginPlay() override;
    
    UPROPERTY(EditDefaultsOnly, Category = "Оружие")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Оружие")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Оружие")
    TSubclassOf<ARSBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Оружие")
    FWeaponSettings WeaponSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Анимация")
    UAnimMontage* EquipAnimMontage;
    
private:

    UPROPERTY()
    ARSBaseWeapon* CurrentWeapon = nullptr;
    
    void AttachWeaponToSocket(ARSBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

    bool CanAim() const;
    bool CanFire() const;
    bool CanMeleeAttack() const;
    bool CanEquip() const;
    bool CanReload() const;

    void OnEmptyClip();
    void ChangeClip();
};
