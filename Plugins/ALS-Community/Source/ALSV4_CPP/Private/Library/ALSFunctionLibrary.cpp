// It is owned by the company Dverg Verksted.


#include "Library/ALSFunctionLibrary.h"

void UALSFunctionLibrary::Print_Log(ELogVerb LogVerb, FText Text, bool bPrintScreen, int Line, const char* Function)
{
	if (LogVerb == ELogVerb::Display)
	{
		UE_LOG(LogLocomotionSystem, Display, TEXT("[%s][%d] Reason:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text.ToString());
	}
	else if (LogVerb == ELogVerb::Warning)
	{
		UE_LOG(LogLocomotionSystem, Warning, TEXT("[%s][%d] Reason:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text.ToString());
	}
	else if (LogVerb == ELogVerb::Error)
	{
		UE_LOG(LogLocomotionSystem, Error, TEXT("[%s][%d] Reason:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text.ToString());
	}

	if (bPrintScreen && GEngine)
	{
#if UE_EDITOR

		if (LogVerb == ELogVerb::Display)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
				(FString("LogLocomotionSystem: Display: [") + ANSI_TO_TCHAR(Function) + "][" + FString::FromInt(Line) + FString(L"] Reason: [") +
					Text.ToString() + FString("]")));
		}
		else if (LogVerb == ELogVerb::Warning)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
				(FString("LogLocomotionSystem: Warning: [") + ANSI_TO_TCHAR(Function) + "][" + FString::FromInt(Line) + FString(L"] Reason: [") +
					Text.ToString() + FString("]")));
		}
		else if (LogVerb == ELogVerb::Error)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
				(FString("LogLocomotionSystem: Error: [") + ANSI_TO_TCHAR(Function) + "][" + FString::FromInt(Line) + FString(L"] Reason: [") +
					Text.ToString() + FString("]")));
		}

#endif
	}
}
