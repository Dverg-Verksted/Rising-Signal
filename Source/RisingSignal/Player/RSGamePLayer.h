// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RSGamePLayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
/**
 * @class Own player (^_^)
 */
UCLASS()
class RISINGSIGNAL_API ARSGamePLayer : public ACharacter
{
    GENERATED_BODY()

public:
    // Def contructor
    ARSGamePLayer();

#pragma region GetData

    /**
     * @public Get Spring Arm component
     * @return USpringArmComponent*
     **/
    UFUNCTION(BlueprintPure, Category = "ARSGamePLayer | GetData")
    USpringArmComponent* GetSpringArm() const { return this->SpringArm; }

    /**
     * @public Get Camera component
     * @return UCameraComponent*
     **/
    UFUNCTION(BlueprintPure, Category = "ARSGamePLayer | GetData")
    UCameraComponent* GetCamera() const { return this->Camera; }

#pragma endregion

protected:
    /**
     *	Function called every frame on this Actor. Override this function to implement custom logic to be executed every frame.
     *	Note that Tick is disabled by default, and you will need to check PrimaryActorTick.bCanEverTick is set to true to enable it.
     *
     *	@param	DeltaSeconds	Game time elapsed during last frame modified by the time dilation
     */
    virtual void Tick(float DeltaSeconds) override;

    virtual void BeginPlay() override;

    // Spring arm component
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArm = nullptr;

    // Spring arm component
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* Camera = nullptr;
};
