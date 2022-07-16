// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/AI/RSAICharacter.h"
#include "RSAICharacter_Wolf.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API ARSAICharacter_Wolf : public ARSAICharacter
{
    GENERATED_BODY()

    ARSAICharacter_Wolf();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;
};
