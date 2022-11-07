// It is owned by the company Dverg Verksted.


#include "Player/NewTestPlayer/RSLedgeDetectorComponent.h"

#include "RSTraceUtility.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"


bool URSLedgeDetectorComponent::LedgeDetect(FLedgeDescription& LedgeDescription)
{
    UCapsuleComponent* CapsuleComponent = CachedCharacterOwner->GetCapsuleComponent();
	ACharacter* DefaultCharacter = GetOwner()->GetClass()->GetDefaultObject<ACharacter>();

	FHitResult ForwardHitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());
	
#if ENABLE_DRAW_DEBUG
	UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryLedgeDetection);
#else
	bool bIsDebugEnabled = false;
#endif
	float DrawTime = 2.0f;
	
	float BotoomZOffset = 2.0f;
	FVector CharacterBotoom = CachedCharacterOwner->GetActorLocation() - (CapsuleComponent->GetScaledCapsuleHalfHeight() - BotoomZOffset) * FVector::UpVector;
	
	float ForwardCheckCapsuleRadius = DefaultCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float ForwardCheckCapsuleHalfHeight = DefaultCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	FVector ForwardStartLocation = CharacterBotoom + (MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	FVector ForwardEndLocation = ForwardStartLocation + CachedCharacterOwner->GetActorForwardVector() * ForwardCheckDistance;

	if(!RSTraceUtility::SweepCapsuleSingleByChanel(GetWorld(), ForwardHitResult, ForwardStartLocation, ForwardEndLocation, FQuat::Identity, ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight, ECC_Climbing, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime))
	{
		return false;
	}
	
	FHitResult DownwardHitResult;
	float DownwardSphereRadius = CapsuleComponent->GetScaledCapsuleRadius();

	float DownwardDepthOffset = 10.0f;
	FVector DownwardStartLocation = ForwardHitResult.ImpactPoint - ForwardHitResult.ImpactNormal * DownwardDepthOffset;
	DownwardStartLocation.Z = CharacterBotoom.Z + MaximumLedgeHeight + DownwardSphereRadius;
	FVector DownwardEndLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBotoom.Z);

	if(!RSTraceUtility::SweepSphereSingleByChanel(GetWorld(), DownwardHitResult, DownwardStartLocation, DownwardEndLocation, DownwardSphereRadius, ECC_GameTraceChannel1, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime))
	{
		return false;
	}

	float OverlapCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float OverlapCapsuleHalfHeight = DefaultCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	float OverlapCapsuleFloorOffset = 10.0f;
	FVector OverlapStartLocation = DownwardHitResult.ImpactPoint + (OverlapCapsuleHalfHeight + OverlapCapsuleFloorOffset) * FVector::UpVector;

	if(RSTraceUtility::OverlapCapsuleBlockingByProfile(GetWorld(), OverlapStartLocation, OverlapCapsuleRadius, OverlapCapsuleHalfHeight, FQuat::Identity, FName("Pawn"), QueryParams, bIsDebugEnabled, DrawTime))
	{
		return false;
	}
	
	LedgeDescription.Location = OverlapStartLocation;
	LedgeDescription.Rotation = (ForwardHitResult.ImpactNormal * FVector(-1.0f, -1.0f, 0)).ToOrientationRotator();
	LedgeDescription.LedgeNormal = ForwardHitResult.ImpactNormal;
	LedgeDescription.PrimitiveComponent = DownwardHitResult.GetComponent();
	LedgeDescription.PrimitiveInitialLocation = DownwardHitResult.GetComponent()->GetComponentLocation();
	
	return true;
}

void URSLedgeDetectorComponent::BeginPlay()
{
	Super::BeginPlay();

    CachedCharacterOwner = StaticCast<ACharacter*>(GetOwner());
}


