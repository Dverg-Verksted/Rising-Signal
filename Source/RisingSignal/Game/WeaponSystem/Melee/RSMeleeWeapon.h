// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/WeaponSystem/RSBaseWeapon.h"
#include "RSMeleeWeapon.generated.h"

UCLASS()
class RISINGSIGNAL_API ARSMeleeWeapon : public ARSBaseWeapon
{
	GENERATED_BODY()

public:
    virtual void StartAttack() override;
    virtual void StopAttack() override;

protected:
    void MakePunch();
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Оружие", DisplayName="Время между взмахами")
    float TimeBetweenSwing = 2.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Оружие", DisplayName="Урон")
    float DamageAmount = 25.f;

    UPROPERTY(EditDefaultsOnly, Category = "Анимация")
    UAnimMontage* AttackAnimMontage;

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    
private:
    void MakeDamage(const FHitResult& HitResult);
    void PlayAnimMontage(UAnimMontage* Animation);
    
    FTimerHandle MeleeTimerHandle;
};
