// It is owned by the company Dverg Verksted.


#include "RSEffect.h"

#include "Library/RSFunctionLibrary.h"
#include "Math/UnitConversion.h"

URSEffect::URSEffect()
{

    
}

void URSEffect::AddEffect(bool const IsDamage, float const EffectVal, float const EffectTime)
{

    NewEffect.IsDamage = IsDamage;
    NewEffect.Value = EffectVal;
    NewEffect.TimeActive = EffectTime;
    ArrEffects.Add(NewEffect);

}

float URSEffect::GetEffectSumValue()
{
    float SumEffectValue = 0.0f;
    for (auto &Effect : ArrEffects)
    {
        if (Effect.TimeActive != 0.0f)
        {
            Effect.TimeActive--;
            if(Effect.IsDamage)
            {
                SumEffectValue += Effect.Value;
            }
            else
            {
                SumEffectValue -= Effect.Value;
            }
        }
    }

    LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Effect value %f"),SumEffectValue));
    return SumEffectValue;
}


