// It is owned by the company Dverg Verksted.


#include "Game/Notifies/FootStepsAnimNotify.h"

#include "AlsAnimationInstance.h"
#include "FMODAudioComponent.h"
#include "FMODBlueprintStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"
#include "Utility/AlsConstants.h"

FString UFootStepsAnimNotify::GetNotifyName_Implementation() const
{

    FString BoneName = StaticEnum<EFootSide>()->GetNameStringByValue(static_cast<int64>(FootBone));
    return FString::Format(TEXT("Footstep Effects: {0}"), {BoneName});
}

void UFootStepsAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    if (!IsValid(MeshComp) || !IsValid(FootstepEffectsSettings))
    {
        return;
    }

    const auto* Character{Cast<ARSBaseCharacter>(MeshComp->GetOwner())};

    if (bSkipEffectsWhenInAir && IsValid(Character) && Character->GetMovementComponent()->IsFalling())
    {
        return;
    }

    const auto CapsuleScale{IsValid(Character) ? Character->GetCapsuleComponent()->GetComponentScale().Z : 1.0f};

    auto* World{MeshComp->GetWorld()};
    const auto* AnimationInstance{Cast<UAlsAnimationInstance>(MeshComp->GetAnimInstance())};

    const auto FootBoneName{FootBone == EFootSide::Left ? UAlsConstants::FootLeftIkBone() : UAlsConstants::FootRightIkBone()};
    const auto FootTransform{MeshComp->GetSocketTransform(FootBoneName)};

    FVector HitLocation;
    FVector HitNormal;
    TWeakObjectPtr<UPrimitiveComponent> HitComponent;
    TWeakObjectPtr<UPhysicalMaterial> HitPhysicalMaterial;

    const auto* FeetState{
        IsValid(AnimationInstance)
            ? (FootBone == EFootSide::Left
                   ? &AnimationInstance->GetFeetState().Left
                   : &AnimationInstance->GetFeetState().Right)
            : nullptr
    };

    if (FeetState != nullptr && FeetState->bOffsetHitValid)
    {
        // Reuse hit information from foot offset logic.

        HitLocation = FeetState->OffsetHitLocation;
        HitNormal = FeetState->OffsetHitNormal;
        HitComponent = FeetState->OffsetHitComponent;
        HitPhysicalMaterial = FeetState->OffsetHitPhysicalMaterial;
    }
    else
    {
        FCollisionQueryParams QueryParameters{ANSI_TO_TCHAR(__FUNCTION__), true, MeshComp->GetOwner()};
        QueryParameters.bReturnPhysicalMaterial = true;

        FHitResult Hit;
        if (World->LineTraceSingleByChannel(Hit, FootTransform.GetLocation(),
            FootTransform.GetLocation() - FVector{
                0.0f, 0.0f, FootstepEffectsSettings->SurfaceTraceDistance * CapsuleScale
            },
            UEngineTypes::ConvertToCollisionChannel(FootstepEffectsSettings->SurfaceTraceChannel),
            QueryParameters))
        {

            HitLocation = Hit.ImpactPoint;
            HitNormal = Hit.ImpactNormal;
            HitComponent = Hit.Component;
            HitPhysicalMaterial = Hit.PhysMaterial;
        }
        else
        {
            HitLocation = FootTransform.GetLocation();
            HitNormal = FVector::UpVector;
            HitComponent = nullptr;
            HitPhysicalMaterial = nullptr;
        }
    }

    const auto SurfaceType{
        HitPhysicalMaterial.IsValid()
            ? HitPhysicalMaterial->SurfaceType
            : TEnumAsByte<EPhysicalSurface>(SurfaceType_Default)
    };

    const FRSFootstepEffectSettings* EffectSettings{nullptr};

    for (const auto& Effect : FootstepEffectsSettings->Effects)
    {
        if (Effect.SurfaceType == SurfaceType)
        {
            EffectSettings = &Effect;
            break;
        }

        if (EffectSettings == nullptr)
        {
            EffectSettings = &Effect;
        }
    }

    if (EffectSettings == nullptr)
    {
        return;
    }

    const auto FootstepLocation{HitLocation};

    const auto FootstepRotation{
        FRotationMatrix::MakeFromZY(HitNormal, FootTransform.TransformVectorNoScale(FootBone == EFootSide::Left
                                                                                        ? FootstepEffectsSettings->FootLeftYAxis
                                                                                        : FootstepEffectsSettings->FootRightYAxis)).
        Rotator()
    };

    if (bSpawnSound)
    {
        auto VolumeMultiplier{SoundVolumeMultiplier};

        // if (!bIgnoreFootstepSoundBlockCurve && IsValid(AnimationInstance))
        // {
        //     VolumeMultiplier *= 1.0f - UAlsMath::Clamp01(AnimationInstance->GetCurveValue(UAlsConstants::FootstepSoundBlockCurve()));
        // }

        if (FAnimWeight::IsRelevant(VolumeMultiplier) && EffectSettings->SoundEvent)
        {
            UFMODAudioComponent* Audio{nullptr};
            FFMODEventInstance EventInstance{nullptr};

            switch (EffectSettings->SoundSpawnType)
            {
                case EFootstepSoundSpawnType::SpawnAtTraceHitLocation:
                {
                    EventInstance = UFMODBlueprintStatics::PlayEventAtLocation(World, EffectSettings->SoundEvent,
                        FTransform{FootstepRotation, FootstepLocation}, false);

                    break;
                }
                case EFootstepSoundSpawnType::SpawnAttachedToFootBone:
                {
                    Audio = UFMODBlueprintStatics::PlayEventAttached(EffectSettings->SoundEvent, MeshComp, FootBoneName,
                        FVector::ZeroVector, EAttachLocation::SnapToTarget, true, false, true);
                    break;
                }
            }

            if (UFMODBlueprintStatics::EventInstanceIsValid(EventInstance))
            {
                UFMODBlueprintStatics::EventInstanceSetParameter(EventInstance, "SurfaceType", SurfaceType == 0 ? 0 : SurfaceType - 1);
                float SpeedNormal = UKismetMathLibrary::NormalizeToRange(Character->GetBaseCharacterMovementComponent()->GetMaxSpeed(), 300,
                    600);
                UFMODBlueprintStatics::EventInstanceSetParameter(EventInstance, "PC_Speed", SpeedNormal);

                UFMODBlueprintStatics::EventInstancePlay(EventInstance);
            }
            else if (IsValid(Audio))
            {
                Audio->SetParameter("SurfaceType", SurfaceType - 1);
                float SpeedNormal = UKismetMathLibrary::NormalizeToRange(Character->GetBaseCharacterMovementComponent()->GetMaxSpeed(), 300,
                    600);
                Audio->SetParameter("PC_Speed", SpeedNormal);

                Audio->Play();

            }
        }
    }

    if (bSpawnDecal && EffectSettings->DecalMaterial.LoadSynchronous())
    {
        const auto DecalRotation{
            FootstepRotation + (FootBone == EFootSide::Left
                                    ? EffectSettings->DecalFootLeftRotationOffset
                                    : EffectSettings->DecalFootRightRotationOffset)
        };

        const auto DecalLocation{FootstepLocation + DecalRotation.RotateVector(EffectSettings->DecalLocationOffset * CapsuleScale)};

        UDecalComponent* Decal;

        if (EffectSettings->DecalSpawnType == EFootstepDecalSpawnType::SpawnAttachedToTraceHitComponent && HitComponent.IsValid())
        {
            Decal = UGameplayStatics::SpawnDecalAttached(EffectSettings->DecalMaterial.Get(), EffectSettings->DecalSize * CapsuleScale,
                HitComponent.Get(), NAME_None, DecalLocation,
                DecalRotation, EAttachLocation::KeepWorldPosition);
        }
        else
        {
            Decal = UGameplayStatics::SpawnDecalAtLocation(World, EffectSettings->DecalMaterial.Get(),
                EffectSettings->DecalSize * CapsuleScale, DecalLocation, DecalRotation);
        }

        if (IsValid(Decal))
        {
            Decal->SetFadeOut(EffectSettings->DecalDuration, EffectSettings->DecalFadeOutDuration, false);
        }
    }

    if (bSpawnParticleSystem && EffectSettings->ParticleSystem)
    {
        switch (EffectSettings->ParticleSystemSpawnType)
        {
            case EFootstepParticleEffectSpawnType::SpawnAtTraceHitLocation:
            {
                const auto ParticleSystemRotation{
                    FootstepRotation + (FootBone == EFootSide::Left
                                            ? EffectSettings->ParticleSystemFootLeftRotationOffset
                                            : EffectSettings->ParticleSystemFootLeftRotationOffset)
                };

                const auto ParticleSystemLocation{
                    FootstepLocation + ParticleSystemRotation.RotateVector(EffectSettings->ParticleSystemLocationOffset * CapsuleScale)
                };

                UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, EffectSettings->ParticleSystem,
                    ParticleSystemLocation, ParticleSystemRotation,
                    FVector::OneVector * CapsuleScale, true, true, ENCPoolMethod::AutoRelease);
            }
            break;

            case EFootstepParticleEffectSpawnType::SpawnAttachedToFootBone: UNiagaraFunctionLibrary::SpawnSystemAttached(
                    EffectSettings->ParticleSystem, MeshComp, FootBoneName,
                    EffectSettings->ParticleSystemLocationOffset * CapsuleScale,
                    EffectSettings->ParticleSystemFootLeftRotationOffset,
                    FVector::OneVector * CapsuleScale, EAttachLocation::KeepRelativeOffset,
                    true, ENCPoolMethod::AutoRelease);
                break;
        }
    }
}
