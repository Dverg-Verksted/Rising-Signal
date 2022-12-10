// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSSearchComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RISINGSIGNAL_API URSSearchComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URSSearchComponent();

protected:
    virtual void BeginPlay() override;


};
