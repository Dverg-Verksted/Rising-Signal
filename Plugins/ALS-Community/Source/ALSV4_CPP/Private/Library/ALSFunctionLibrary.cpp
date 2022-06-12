// It is owned by the company Dverg Verksted.

#include "Library/ALSFunctionLibrary.h"

void UALSFunctionLibrary::Print_Log(ELogVerb LogVerb, FString Text, int Line, const char* Function)
{
	if (LogVerb == ELogVerb::Display)
	{
		UE_LOG(LogLocomotionSystem, Display, TEXT("[%s][%d] Reason:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
	}
	else if (LogVerb == ELogVerb::Warning)
	{
		UE_LOG(LogLocomotionSystem, Warning, TEXT("[%s][%d] Reason:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
	}
	else if (LogVerb == ELogVerb::Error)
	{
		UE_LOG(LogLocomotionSystem, Error, TEXT("[%s][%d] Reason:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
	}
}
