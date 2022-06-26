// It is owned by the company Dverg Verksted.

#include "Library/RSFunctionLibrary.h"

void URSFunctionLibrary::Print_Log(ELogRSVerb LogVerb, FString Text, int Line, const char* Function)
{
    if (LogVerb == ELogRSVerb::Display)
    {
        UE_LOG(LogRisingSignal, Display, TEXT("[%s][%d] TEXT:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
    }
    else if (LogVerb == ELogRSVerb::Warning)
    {
        UE_LOG(LogRisingSignal, Warning, TEXT("[%s][%d] TEXT:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
    }
    else if (LogVerb == ELogRSVerb::Error)
    {
        UE_LOG(LogRisingSignal, Error, TEXT("[%s][%d] TEXT:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
    }
}
