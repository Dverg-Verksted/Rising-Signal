// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RSSavableObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class URSSavableObjectInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class RISINGSIGNAL_API IRSSavableObjectInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /* Called after the Actor state was restored from a SaveGame file. */
    UFUNCTION(BlueprintNativeEvent)
    void OnActorLoaded();
};
