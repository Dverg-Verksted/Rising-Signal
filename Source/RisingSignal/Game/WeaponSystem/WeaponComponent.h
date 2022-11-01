// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class ARSBaseWeapon;

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ARSBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* AnimMontage;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RISINGSIGNAL_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

    void StartAiming();
    void StopAiming();
    void StartAttack();
    void StopAttack();
    
    void NextWeapon();
    void Reload();

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

private:

    UPROPERTY()
    ARSBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ARSBaseWeapon*> Weapons;

    void SpawnWeapons();
    void AttachWeaponToSocket(ARSBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

    bool CanAim() const;
    bool CanFire() const;
    bool CanMeleeAttack() const;
    bool CanEquip() const;
    bool CanReload() const;

    void OnEmptyClip();
    void ChangeClip();
};
