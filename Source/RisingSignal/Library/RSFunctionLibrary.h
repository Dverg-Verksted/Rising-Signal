// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RSFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class ELogRSVerb : uint8
{
    Display,
    Warning,
    Error
};

DEFINE_LOG_CATEGORY_STATIC(LogRisingSignal, All, All);
#define LOG_RS(LogVerb, Text) URSFunctionLibrary::Print_Log(LogVerb, Text, __LINE__, __FUNCTION__)

/**
 * @class Generalized RS Functional Library
 */
UCLASS()
class RISINGSIGNAL_API URSFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * @public Write a log
     **/
    static void Print_Log(ELogRSVerb LogVerb, FString Text, int Line, const char* Function);
};
