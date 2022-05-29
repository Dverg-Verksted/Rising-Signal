// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "ALSCharacterEnumLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ALSFunctionLibrary.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogLocomotionSystem, All, All);
#define LOG_ALS(LogVerb, Text, State) UALSFunctionLibrary::Log_Print(LogVerb, Text, State, __LINE__, __FUNCTION__)

/**
 * @class Generalized ALS Functional Library
 */
UCLASS()
class ALSV4_CPP_API UALSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * @public Write a log
	 **/
	static void Print_Log(ELogVerb LogVerb, FText Text, bool bPrintScreen, int Line, const char* Function);
};
