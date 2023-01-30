/**
  * Maintain: Mark Veligod
  * GitHub: https://github.com/markveligod
  * Itch: https://markveligod.itch.io/
 **/

#include "Game/UIMenu/Library/HUDMSFunctionLibrary.h"

bool UMSFunctionLibrary::Print_Exp(bool Var, FString Str, int Line, const char* Function)
{
    if (!Var)
    {
        Print_Log(ELogVerb::Error, Str, Line, Function);
        return false;
    }
    return true;
}

void UMSFunctionLibrary::Print_Log(ELogVerb TypeVerb, FString Str, int Line, const char* Function)
{
    if (TypeVerb == ELogVerb::Display)
    {        
        UE_LOG(LogMenuSystem, Display, TEXT("[%s][%d] Reason:[%s]"), ANSI_TO_TCHAR(Function), Line, *Str);
    }
    else if (TypeVerb == ELogVerb::Warning)
    {
        UE_LOG(LogMenuSystem, Warning, TEXT("[%s][%d] Reason:[%s]"), ANSI_TO_TCHAR(Function), Line, *Str);
    }
    else if (TypeVerb == ELogVerb::Error)
    {
        UE_LOG(LogMenuSystem, Error, TEXT("[%s][%d] Reason:[%s]"), ANSI_TO_TCHAR(Function), Line, *Str);
    }

#if UE_EDITOR

    if (GEngine)
    {
        if (TypeVerb == ELogVerb::Display && StateShowLog == EMSStateShowLog::All)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
                (FString("LogMenuSystem: Display: [") + ANSI_TO_TCHAR(Function) + "][" + FString::FromInt(Line) +
                    FString(L"] Reason: [") + Str + FString("]")));
        }
        else if (TypeVerb == ELogVerb::Warning && StateShowLog >= EMSStateShowLog::Warning)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
                    (FString("LogMenuSystem: Warning: [") + ANSI_TO_TCHAR(Function) + "][" + FString::FromInt(Line) +
                        FString(L"] Reason: [") + Str + FString("]")));
        }
        else if (TypeVerb == ELogVerb::Error && StateShowLog >= EMSStateShowLog::Error)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                    (FString("LogMenuSystem: Error: [") + ANSI_TO_TCHAR(Function) + "][" + FString::FromInt(Line) +
                        FString(L"] Reason: [") + Str + FString("]")));
        }
    }
    
#endif
}

FIntPoint UMSFunctionLibrary::GetSizeScreenFromString(const FString Str)
{
    FIntPoint Point;

    if (Str.IsEmpty()) return Point;

    TArray<FString> TempParseArray;
    Str.ParseIntoArray(TempParseArray, TEXT("x"));
    if (TempParseArray.Num() < 2) return Point;
    
    Point.X = FCString::Atoi(*TempParseArray[0]);
    Point.Y = FCString::Atoi(*TempParseArray[1]);

    return Point;
}
