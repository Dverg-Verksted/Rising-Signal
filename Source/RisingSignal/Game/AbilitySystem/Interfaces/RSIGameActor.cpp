 #include "RSIGameActor.h"


// Add default functionality here for any IRSIGameActor functions that are not pure virtual.
EPhysicalSurface IRSIGameActor::GetSurfaceType()
{
	return EPhysicalSurface::SurfaceType_Default;
}

TArray<URSStateEffects*> IRSIGameActor::GetAllCurrentEffects()
{
	TArray<URSStateEffects*> Effect;
	return Effect;
}

void IRSIGameActor::RemoveEffect(URSStateEffects* RemoveEffect)
{
}

void IRSIGameActor::AddEffect(URSStateEffects* newEffect)
{
}

FVector IRSIGameActor::GetEffectLocation(URSStateEffects* Effect)
{
	return FVector(0);
}

FString IRSIGameActor::GetAttachComponent(URSStateEffects* Effect)
{
	return Effect->EffectOwnerActor->GetRootComponent()->GetName();
}
