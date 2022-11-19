// It is owned by the company Dverg Verksted.


#include "Player/NewTestPlayer/RSTraceUtility.h"

bool RSTraceUtility::SweepCapsuleSingleByChanel(const UWorld* World, FHitResult& OutHit, const FVector& Start, const FVector& End,
    const FQuat& Rot, float CapsuleRadius, float CapsuleHalfHeight, ECollisionChannel TraceChanel, const FCollisionQueryParams& QueryParams,
    const FCollisionResponseParams& ResponseParams, float DrawTime, FColor TraceColor, FColor HitColor)
{
    bool bResult;
    	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
    	bResult = World->SweepSingleByChannel(OutHit, Start, End, Rot, TraceChanel, CollisionShape, QueryParams, ResponseParams);
    	
    /*#if ENABLE_DRAW_DEBUG
    	if(bDrawDebug)
    	{
    		DrawDebugCapsule(World, Start, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, TraceColor, false, DrawTime);
    		DrawDebugCapsule(World, End, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, TraceColor, false, DrawTime);
    		DrawDebugLine(World, Start, End, FColor::Black, false, 2.0f);
    
    		if(bResult)
    		{
    			DrawDebugCapsule(World, OutHit.Location, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, HitColor, false, DrawTime);
    			DrawDebugPoint(World, OutHit.ImpactPoint, 10.0f, FColor::Red, false, DrawTime);
    		}
    	}
    #endif*/
    	
    	return bResult;
}

bool RSTraceUtility::SweepSphereSingleByChanel(const UWorld* World, FHitResult& OutHit, const FVector& Start, const FVector& End,
    float Radius, ECollisionChannel TraceChanel, const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams,
    float DrawTime, FColor TraceColor, FColor HitColor)
{
    bool bResult;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
    bResult = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, TraceChanel, CollisionShape, QueryParams, ResponseParams);
	
/*#if ENABLE_DRAW_DEBUG
    if(bDrawDebug)
    {
        FVector DebugDrawCapsuleLocation = (Start + End) * 0.5f;
        FVector TraceVector = End - Start;
        float DebugDrawCapsuleHalfHeight = TraceVector.Size() * 0.5f;
        FQuat DebugCapsuleRotation = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();

        DrawDebugCapsule(World, DebugDrawCapsuleLocation, DebugDrawCapsuleHalfHeight, Radius, DebugCapsuleRotation, TraceColor, false, DrawTime);

        if(bDrawDebug)
        {
            DrawDebugSphere(World, OutHit.Location, Radius, 32, HitColor, false, DrawTime);
            DrawDebugPoint(World, OutHit.ImpactPoint, 10.0f, HitColor, false, DrawTime);
        }
    }
#endif*/
	
    return bResult;
}

bool RSTraceUtility::OverlapCapsuleAnyByProfile(const UWorld* World, const FVector Pos, float CapsuleRadius, float CapsuleHalfHeight,
    FQuat Rotation, FName ProfileName, const FCollisionQueryParams& QueryParams, float DrawTime, FColor HitColor)
{
    bool bResult;
    FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
    bResult = World->OverlapAnyTestByProfile(Pos, Rotation, ProfileName, CollisionShape, QueryParams);
	
/*#if ENABLE_DRAW_DEBUG
    if(bDrawDebug && bResult)
    {
        DrawDebugCapsule(World, Pos, CapsuleHalfHeight, CapsuleRadius, Rotation, HitColor, false, DrawTime);
    }
#endif*/
	
    return bResult;
}

bool RSTraceUtility::OverlapCapsuleBlockingByProfile(const UWorld* World, const FVector Pos, float CapsuleRadius, float CapsuleHalfHeight,
    FQuat Rotation, FName ProfileName, const FCollisionQueryParams& QueryParams, float DrawTime, FColor HitColor)
{
    bool bResult;
    FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
    bResult = World->OverlapBlockingTestByProfile(Pos, Rotation, ProfileName, CollisionShape, QueryParams);
	
/*#if ENABLE_DRAW_DEBUG
    if(bDrawDebug && bResult)
    {
        DrawDebugCapsule(World, Pos, CapsuleHalfHeight, CapsuleRadius, Rotation, HitColor, false, DrawTime);
    }
#endif*/
	
    return bResult;
}
