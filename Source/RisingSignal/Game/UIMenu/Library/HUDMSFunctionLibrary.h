/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#pragma once

#include "CoreMinimal.h"
#include "Game/UIMenu/Base/MSDataTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HUDMSFunctionLibrary.generated.h"

UENUM(Blueprintable)
enum class ELogVerb: uint8
{
    Display,
    Warning,
    Error
};

DEFINE_LOG_CATEGORY_STATIC(LogMenuSystem, All, All);

#define CHECK(exp, str) UMSFunctionLibrary::Print_Exp(exp, str, __LINE__, __FUNCTION__)
#define LOGJAM(verb, str) UMSFunctionLibrary::Print_Log(verb, str, __LINE__, __FUNCTION__)
#define VALUE_PERCENT 100.0f

//
EMSStateShowLog StateShowLog = EMSStateShowLog::Error;

/**
 * @class Functional library for project support
 */
UCLASS()
class RISINGSIGNAL_API UMSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    
    // @public Checking expressions for print log
    static bool Print_Exp(bool Var, FString Str, int Line, const char* Function);
    
    // @public Print a log on screen(UE_EDITOR only) and in ULOG
    static void Print_Log(ELogVerb TypeVerb, FString Str, int Line, const char* Function);

    // @public Get string from bool
    UFUNCTION(BlueprintPure, Category = "MSFunctionLibrary")
    static FString GetStringFromBool(const bool State) { return State ? FString("true") : FString("false"); }

    // @public Get string size screen from FIntPoint
    UFUNCTION(BlueprintPure, Category = "MSFunctionLibrary")
    static FString GetStringSizeScreen(const FIntPoint Point) { return FString::FromInt(Point.X) + "x" + FString::FromInt(Point.Y); }

    // @public Get FIntPoint size screen from string
    UFUNCTION(BlueprintPure, Category = "MSFunctionLibrary")
    static FIntPoint GetSizeScreenFromString(const FString Str);
};
