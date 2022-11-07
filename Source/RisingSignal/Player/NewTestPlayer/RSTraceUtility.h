// It is owned by the company Dverg Verksted.

#pragma once


namespace RSTraceUtility
{
    bool SweepCapsuleSingleByChanel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start, const FVector& End, const FQuat& Rot, float CapsuleRadius, float CapsuleHalfHeight, ECollisionChannel TraceChanel, const FCollisionQueryParams& QueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& ResponseParams = FCollisionResponseParams::DefaultResponseParam, bool bDrawDebug = false, float DrawTime = -1.0f, FColor TraceColor = FColor::Black, FColor HitColor = FColor::Red);

    bool SweepSphereSingleByChanel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start, const FVector& End, float Radius, ECollisionChannel TraceChanel, const FCollisionQueryParams& QueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& ResponseParams = FCollisionResponseParams::DefaultResponseParam, bool bDrawDebug = false, float DrawTime = -1.0f, FColor TraceColor = FColor::Black, FColor HitColor = FColor::Red);

    bool OverlapCapsuleAnyByProfile(const UWorld* World, const FVector Pos, float CapsuleRadius, float CapsuleHalfHeight, FQuat Rotation, FName ProfileName, const FCollisionQueryParams& QueryParams, bool bDrawDebug = false, float DrawTime = -1.0f, FColor HitColor = FColor::Red);

    bool OverlapCapsuleBlockingByProfile(const UWorld* World, const FVector Pos, float CapsuleRadius, float CapsuleHalfHeight, FQuat Rotation, FName ProfileName, const FCollisionQueryParams& QueryParams, bool bDrawDebug = false, float DrawTime = -1.0f, FColor HitColor = FColor::Red);
}

