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
    NewEffect->Value = EffectVal;
    NewEffect->TimeActive = EffectTime;
    ArrEffects.Add(NewEffect);
    LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("%d"),ArrEffects.Num()));

}

float URSEffect::GetEffectSumValue()
{
    float SumEffectValue = 0.0f;
    for (const auto Effect : ArrEffects)
    {
        if(Effect->IsDamage)
        {
            SumEffectValue -= Effect->Value;
        }
        else
        {
            SumEffectValue += Effect->Value;
        }

        Effect->TimeActive--;
        
        if (Effect->TimeActive <= 0)
        {
            ArrEffects.Remove(Effect);
        }
    }

    return SumEffectValue;
}

