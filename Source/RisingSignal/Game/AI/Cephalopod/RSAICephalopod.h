// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/AI/RSAICharacter.h"
#include "RSAICephalopod.generated.h"

/**
 * 
 */
UCLASS(HideCategories = ("Variable", "Transform", "Sockets", "Shape", "Navigation", "ComponentTick", "Physics", "Tags", "Cooking", "HLOD",
        "Mobile", "Activation", "Component Replication", "Events", "Asset User Data", "Collision"))
class RISINGSIGNAL_API ARSAICephalopod : public ARSAICharacter
{
    GENERATED_BODY()

public:
    ARSAICephalopod();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    virtual void AIStateChanged(EAIState NewState, EAIState PrevState) override;
    virtual void Attack(AActor* ActorToAttack) override;
    virtual void ProvideDamage(USkeletalMeshComponent* FromMeshComponent) override;

protected:
};
