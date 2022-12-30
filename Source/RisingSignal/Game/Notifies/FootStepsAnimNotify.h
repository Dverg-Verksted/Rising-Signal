// It is owned by the company Dverg Verksted.

#pragma once


// #include "Chaos/ChaosEngineInterface.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"

#include "Animation/AnimNotifies/AnimNotify.h"
#include "FootStepsAnimNotify.generated.h"

class USoundBase;
class UMaterialInterface;
class UNiagaraSystem;

UENUM()
enum class EFootSide : uint8
{
    Left,
    Right
};

UENUM()
enum class ESpeedParam : uint8
{
    Walk,
    Run
};

UENUM(BlueprintType)
enum class EFootstepSoundSpawnType : uint8
{
    SpawnAtTraceHitLocation,
    SpawnAttachedToFootBone
};

UENUM(BlueprintType)
enum class EFootstepDecalSpawnType : uint8
{
    SpawnAtTraceHitLocation,
    SpawnAttachedToTraceHitComponent
};

UENUM()
enum class EFootstepParticleEffectSpawnType : uint8
{
    SpawnAtTraceHitLocation,
    SpawnAttachedToFootBone
};


USTRUCT(BlueprintType)
struct FRSFootstepEffectSettings
{
    GENERATED_BODY()


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<EPhysicalSurface> SurfaceType{SurfaceType1};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    class UFMODEvent* SoundEvent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    EFootstepSoundSpawnType SoundSpawnType{EFootstepSoundSpawnType::SpawnAtTraceHitLocation};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
    TSoftObjectPtr<UMaterialInterface> DecalMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPreserveRatio))
    FVector DecalSize{10.0f, 20.0f, 20.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0, ForceUnits = "s"))
    float DecalDuration{4.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0, ForceUnits = "s"))
    float DecalFadeOutDuration{2.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
    EFootstepDecalSpawnType DecalSpawnType{EFootstepDecalSpawnType::SpawnAttachedToTraceHitComponent};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
    FVector DecalLocationOffset{0.0f, -10.0f, -1.75f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
    FRotator DecalFootLeftRotationOffset{90.0f, -90.0f, 180.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
    FRotator DecalFootRightRotationOffset{-90.0f, 90.0f, 0.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
    UNiagaraSystem* ParticleSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
    EFootstepParticleEffectSpawnType ParticleSystemSpawnType{EFootstepParticleEffectSpawnType::SpawnAtTraceHitLocation};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
    FVector ParticleSystemLocationOffset{ForceInit};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
    FRotator ParticleSystemFootLeftRotationOffset{ForceInit};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
    FRotator ParticleSystemFootRightRotationOffset{ForceInit};

};

UCLASS(Blueprintable, BlueprintType)
class RISINGSIGNAL_API URSFootstepEffectsSettings : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<ETraceTypeQuery> SurfaceTraceChannel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ForceUnits = "cm"))
    float SurfaceTraceDistance{50.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Foot Left Y Axis")
    FVector FootLeftYAxis{0.0f, 0.0f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Foot Right Y Axis")
    FVector FootRightYAxis{0.0f, 0.0f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "SurfaceType"))
    TArray<FRSFootstepEffectSettings> Effects;
};

UCLASS()
class RISINGSIGNAL_API UFootStepsAnimNotify : public UAnimNotify
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
    URSFootstepEffectsSettings* FootstepEffectsSettings;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
    EFootSide FootBone{EFootSide::Left};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
    bool bSkipEffectsWhenInAir;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
    bool bSpawnSound{true};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
    float SoundVolumeMultiplier{1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
    float SoundPitchMultiplier{1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
    ESpeedParam SoundType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
    bool bIgnoreFootstepSoundBlockCurve;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Decal", Meta = (AllowPrivateAccess))
    bool bSpawnDecal{true};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Particle System", Meta = (AllowPrivateAccess))
    bool bSpawnParticleSystem{true};

public:
    virtual FString GetNotifyName_Implementation() const override;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


};
