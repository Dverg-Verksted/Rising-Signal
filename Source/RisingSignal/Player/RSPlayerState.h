// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RSPlayerState.generated.h"


class URSSaveGame;


UCLASS()
class RISINGSIGNAL_API ARSPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent)
    void SavePlayerState(URSSaveGame* SaveObject);

    UFUNCTION(BlueprintNativeEvent)
    void LoadPlayerState(URSSaveGame* SaveObject);
};
