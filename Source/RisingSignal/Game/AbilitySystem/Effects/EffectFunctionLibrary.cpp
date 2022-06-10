// It is owned by the company Dverg Verksted.


#include "Game/AbilitySystem/Effects/EffectFunctionLibrary.h"
#include "Game/AbilitySystem/Interfaces/RSIGameActor.h"

void UMyBlueprintFunctionLibrary::AddEffectBySurfaceType(AActor* TakeEffectActor, TSubclassOf<URSStateEffects> AddEffectClass,
	EPhysicalSurface SurfaceType, FName NameBoneHit)
{
	if (SurfaceType != EPhysicalSurface::SurfaceType_Default && TakeEffectActor && AddEffectClass)
	{
		URSStateEffects* myEffect = Cast<URSStateEffects>(AddEffectClass->GetDefaultObject());
		if (myEffect)
		{
			bool bIsHavePossibleSurface = false;
			int8 i = 0;
			while (i < myEffect->PossibleInteractSurface.Num() && !bIsHavePossibleSurface)
			{
				if (myEffect->PossibleInteractSurface[i] == SurfaceType)
				{			
					bIsHavePossibleSurface = true;
					bool bIsCanAddEffect = false;
					if (!myEffect->bIsStackable)
					{
						int8 j = 0;
						TArray<URSStateEffects*> CurrentEffects;
						IRSIGameActor* myInterface = Cast<IRSIGameActor>(TakeEffectActor);
						if (myInterface)
						{
							CurrentEffects = myInterface->GetAllCurrentEffects();
						}

						if (CurrentEffects.Num() > 0)
						{
							while (j < CurrentEffects.Num() && !bIsCanAddEffect)
							{
								if (CurrentEffects[j]->GetClass() != AddEffectClass)
								{
									bIsCanAddEffect = true;
								}
								j++;
							}
						}
						else
						{
							bIsCanAddEffect = true;
						}
						
					}
					else
					{
						bIsCanAddEffect = true;
					}

					if (bIsCanAddEffect)
					{
						
						URSStateEffects* NewEffect = NewObject<URSStateEffects>(TakeEffectActor, AddEffectClass);
						if (NewEffect)
						{
							NewEffect->InitObject(TakeEffectActor, NameBoneHit);
						}
					}
				}
				i++;
			}
		}
	}
}
