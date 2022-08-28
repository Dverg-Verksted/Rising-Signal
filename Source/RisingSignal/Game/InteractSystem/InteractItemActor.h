// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "InteractDataItem.h"
#include "Engine/DataTable.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "GameFramework/Actor.h"
#include "InteractItemActor.generated.h"


class UWidgetComponent;
class UInteractWidget;
UCLASS(HideCategories = ("Variable", "Transform", "Sockets", "Shape", "Navigation", "ComponentTick", "Physics", "Tags", "Cooking", "HLOD",
    "Mobile", "Activation", "Component Replication", "Events", "Asset User Data", "Collision", "Rendering", "Input", "Actor", "LOD"))
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

    /**
     * @public get data asset interact item
     * @return UInteractItemDataAsset*
     **/
    UFUNCTION(BlueprintPure, Category = "AInteractItemActor | DataInteractItem")
    FDataTableRowHandle GetInteractData() const { return InteractData; };

    /**
     * Temp function. Will be removed
     **/
    UFUNCTION(BlueprintPure, Category = "AInteractItemActor | DataInteractItem")
    FDataInteract GetItemData() const
    {
        if (InteractData.DataTable && InteractData.RowName != "None")
            return *InteractData.DataTable->FindRow<FDataInteract>(InteractData.RowName, "");

        return FDataInteract();
    };

    UFUNCTION(BlueprintPure, Category = "AInteractItemActor | DataInteractItem")
    int32 GetItemCount() const { return ItemCount; };

private:
    // @private pointer on InteractItemDataAsset
    UPROPERTY(EditDefaultsOnly, Category = "Settings Interact")
    UDataTable* DataTableInteractItem;

    // @private pointer on InteractItemDataAsset
    UPROPERTY(EditAnywhere, Category = "Settings Interact", meta = (DisplayName = "Виджет интеракта",
        ToolTip = "Укажите виджет интеракта если требуется"))
    TSubclassOf<UInteractWidget> SubInteractWidget;

    // @private pointer on InteractItemDataAsset
    UPROPERTY(EditAnywhere, Category = "Settings Interact", meta = (DisplayName = "Данные об предмете",
        ToolTip = "Укажите данные об предмете из таблицы"))
    FDataTableRowHandle InteractData;

    // @private Type item
    UPROPERTY(VisibleInstanceOnly, Category = "Settings Interact", meta = (DisplayName = "Тип предмета"))
    ETypeItem TypeItem = ETypeItem::None;

    // @private Inventory item count
    UPROPERTY(EditInstanceOnly, Category = "Settings Interact",
        meta = (DisplayName = "Количество предметов", EditCondition = "TypeItem == ETypeItem::InvItem", EditConditionHides,
            ClampMin = 0, ClampMax = 100, AllowPrivateAccess))
    int32 ItemCount = 1;

    // @private Name item
    UPROPERTY(VisibleInstanceOnly, Category = "Settings Interact",
        meta = (DisplayName = "Имя предмета"))
    FText NameItem = FText();

    // @private Description item
    UPROPERTY(VisibleInstanceOnly, Category = "Settings Interact",
        meta = (DisplayName = "Описание предмета", MultiLine))
    FText DescriptionItem = FText();

    UPROPERTY()
    UInteractWidget* InteractWidget;

    FTimerHandle ResetInteractAnimTimerHandle;


#pragma endregion

#pragma region Statics
public:
    UFUNCTION(BlueprintCallable)
    static void SpawnItem(AActor* Spawner, FInventoryItem InventoryItemRules, float Distance, int32 Count);

    static FName GetRowNameByItemName(const UDataTable* DataTable, FText ItemName);

#pragma endregion
};
