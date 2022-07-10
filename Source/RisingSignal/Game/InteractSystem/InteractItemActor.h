// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractItemActor.generated.h"

class UWidgetComponent;
class UInteractWidget;
class UInteractItemDataAsset;
UCLASS()
class RISINGSIGNAL_API AInteractItemActor : public AActor
{
    GENERATED_BODY()

#pragma region Default

public:
    // Sets default values for this actor's properties
    AInteractItemActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

#if UE_EDITOR

    // Called when changed property
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

#pragma endregion

#pragma region Components

private:
    
    // @private Mesh component
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    // @private 3D Widget component
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UWidgetComponent* WidgetComponent;

#pragma endregion

#pragma region DataInteractItem

public:

    /**
     * @public Load interact widget
     **/
    UFUNCTION(BlueprintCallable, Category = "AInteractItemActor | DataInteractItem")
    void LoadInteractWidget();

    /**
     * @public Destroy interact widget
     **/
    UFUNCTION(BlueprintCallable, Category = "AInteractItemActor | DataInteractItem")
    void DestroyInteractWidget();
    
private:

    // @private pointer on InteractItemDataAsset
    UPROPERTY(EditAnywhere, Category = "Settings Interact", meta = (DisplayName = "Виджет интеракта",
        ToolTip = "Укажите виджет интеракта если требуется"))
    TSubclassOf<UInteractWidget> SubInteractWidget;

    // @private pointer on InteractItemDataAsset
    UPROPERTY(EditAnywhere, Category = "Settings Interact", meta = (DisplayName = "Данные об предмете",
        ToolTip = "Укажите данные об предмете"))
    UInteractItemDataAsset* InteractItem;
    
    UPROPERTY()
    UInteractWidget* InteractWidget;

    FTimerHandle ResetInteractAnimTimerHandle;
    

#pragma endregion

};
