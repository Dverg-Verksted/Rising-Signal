// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RSCharState.generated.h"



/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API URSCharState : public UObject
{
    GENERATED_BODY()

public:

    // constructor
    URSCharState();

    UFUNCTION()
    void ChangeChangedValue(float ChangedValue);

    UFUNCTION()
    void ChangeCurrentValue(float ChangedValue);

    
protected:

    
private:

    
    
};
    