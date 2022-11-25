// It is owned by the company Dverg Verksted.


#include "Player/NewTestPlayer/RSTraceUtility.h"

bool RSTraceUtility::SweepCapsuleSingleByChanel(const UWorld* World, FHitResult& OutHit, const FVector& Start, const FVector& End,
    const FQuat& Rot, float CapsuleRadius, float CapsuleHalfHeight, ECollisionChannel TraceChanel, const FCollisionQueryParams& QueryParams,
    const FCollisionResponseParams& ResponseParams, float DrawTime, FColor TraceColor, FColor HitColor)
{
    bool bResult;
    	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
    	bResult = World->SweepSingleByChannel(OutHit, Start, End, Rot, TraceChanel, CollisionShape, QueryParams, ResponseParams);
    	
    	return bResult;
}

bool RSTraceUtility::SweepSphereSingleByChanel(const UWorld* World, FHitResult& OutHit, const FVector& Start, const FVector& End,
    float Radius, ECollisionChannel TraceChanel, const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams,
    float DrawTime, FColor TraceColor, FColor HitColor)
{
    bool bResult;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
    bResult = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, TraceChanel, CollisionShape, QueryParams, ResponseParams);
    
	
    return bResult;
}

bool RSTraceUtility::OverlapCapsuleAnyByProfile(const UWorld* World, const FVector Pos, float CapsuleRadius, float CapsuleHalfHeight,
    FQuat Rotation, FName ProfileName, const FCollisionQueryParams& QueryParams, float DrawTime, FColor HitColor)
{
    bool bResult;
    FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
    bResult = World->OverlapAnyTestByProfile(Pos, Rotation, ProfileName, CollisionShape, QueryParams);
	

	
    return bResult;
}

bool RSTraceUtility::OverlapCapsuleBlockingByProfile(const UWorld* World, const FVector Pos, float CapsuleRadius, float CapsuleHalfHeight,
    FQuat Rotation, FName ProfileName, const FCollisionQueryParams& QueryParams, float DrawTime, FColor HitColor)
{
    bool bResult;
    FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
    bResult = World->OverlapBlockingTestByProfile(Pos, Rotation, ProfileName, CollisionShape, QueryParams);
    
	
    return bResult;
}
