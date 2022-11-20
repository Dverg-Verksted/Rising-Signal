// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class RISINGSIGNAL_API ARSBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ARSBaseWeapon();
    
    virtual void StartAiming();
    virtual void StopAiming();
    virtual void StartAttack();
    virtual void StopAttack();
    APlayerController* GetPlayerController() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Компоненты", DisplayName="Скелетал меш оружия" )
    USkeletalMeshComponent* WeaponMesh;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Оружие", DisplayName="Сокет оружия")
    FName MuzzleSocketName = "MuzzleSocket";
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Оружие", DisplayName="Максимальная дальность трейс луча")
    float TraceMaxDistance = 1000.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Инвентарь", DisplayName="Прочность",
            meta=(EditCondition="bIsWeapon", EditConditionHides, UIMin = 0.0f, ClampMin = 0.0f, UIMax = 100.0f, ClampMax = 100.0f))
    float ItemsDurability = 0.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Инвентарь", DisplayName="Можно снарядить?")
    bool bIsWeapon = true;

	virtual void BeginPlay() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    FVector GetMuzzleWorldLocation() const;
    
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
};
