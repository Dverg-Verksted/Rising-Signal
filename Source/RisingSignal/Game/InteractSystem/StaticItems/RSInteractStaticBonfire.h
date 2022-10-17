// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/InteractSystem/RSInteractStaticItemBase.h"
#include "RSInteractStaticBonfire.generated.h"

class AInteractItemActor;
class UNiagaraSystem;
class USphereComponent;


/**
 * 
 */
UCLASS(HideCategories = ("Variable", "Transform", "Sockets", "Shape", "Navigation", "ComponentTick", "Physics", "Tags", "Cooking", "HLOD",
    "Mobile", "Activation", "Component Replication", "Events", "Asset User Data", "Collision", "Rendering", "Input", "Actor", "LOD"))
class RISINGSIGNAL_API ARSInteractStaticBonfire : public ARSInteractStaticItemBase
{
    GENERATED_BODY()

#pragma region Public_Defaults

public:
    virtual void Interact(ACharacter* InteractingCharacter) override;

    /**
     * Toggles fire effects 
     */
    void SetFire(bool bSetFire);

#pragma endregion Public_Defaults

protected:
#pragma region Protected_Defaults

    ARSInteractStaticBonfire();

    virtual void BeginPlay() override;


    UFUNCTION()
    void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    /**
     * Increase or decrease character temperature based on bHeatUp value
     * @param Character Character to whom change temperature
     * @param bCharInside marks that character inside or not
     */
    void CharacterInsideVolume(ACharacter* Character, const bool bCharInside);

    /**
     * Toggle Fire and Smoke VFXs 
     */
    void SetEnabledVFX(bool bEnable);

#if UE_EDITOR

    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

    void CheckIfCharactersInsideVolume();

#pragma endregion Protected_Defaults


#pragma region Protected_Components
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* BaseMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USphereComponent* HeatVolume;

    // UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Settings | VFX")
    // UParticleSystemComponent* ParticleSystemComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UParticleSystemComponent* FireVFX;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UParticleSystemComponent* SmokeVFX;


#pragma endregion Protected_Components


#pragma region Protected_Properties

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", DisplayName = "Тип костра",
        meta = (ToolTip =
            "Малый костер - костер без котла, с ограниченными рецептами.\nБольшой костер - костер с котлом, все рецепты для костра"
        ))
    EBonfireType BonfireType = EBonfireType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | HeatVolume")
    float HeatVolumeRadius = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | HeatVolume",
        meta = (
            DisplayName = "Отогревание в секунду",
            ToolTip = "На сколько будет повышаться температура игрока в секунду"
        ))
    float HeatTemperIncreaseValue = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings",
        meta = (
            DisplayName = "Горит",
            ToolTip = "Должен ли этот костер гореть?"
        ))
    bool bIsFired = false;

    UPROPERTY()
    AInteractItemActor* ParentInteractActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings",
        meta = (
            DisplayName = "Текст без огня",
            ToolTip = "Текст, который будет выведен у негорящего костра"
        ))
    FText FireOffText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings",
        meta = (
            DisplayName = "Текст с огнем",
            ToolTip = "Текст, который будет выведен у горящего костра"
        ))
    FText FireOnText;

#pragma endregion Protected_Properties
};
