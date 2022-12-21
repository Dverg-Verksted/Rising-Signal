﻿// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "InteractDataItem.h"
#include "Components/TimelineComponent.h"
#include "Engine/DataTable.h"
#include "Game/Inventory/InventoryTypes.h"
#include "GameFramework/Actor.h"
#include "Game/SaveLoad/RSSavableObjectInterface.h"
#include "InteractItemActor.generated.h"


class USphereComponent;
class UWidgetComponent;
class UInteractWidget;
UCLASS(HideCategories = ("Variable", "Transform", "Sockets", "Shape", "Navigation", "ComponentTick", "Physics", "Tags", "Cooking", "HLOD",
    "Mobile", "Activation", "Component Replication", "Events", "Asset User Data", "Collision", "Rendering", "Input", "Actor", "LOD"))
class RISINGSIGNAL_API AInteractItemActor : public AActor, public IRSSavableObjectInterface
{
    GENERATED_BODY()

#pragma region Default

public:
    // Sets default values for this actor's properties
    AInteractItemActor();

protected:
    virtual void BeginPlay() override;

    virtual void PostLoad() override;

#if UE_EDITOR

    // Called when changed property
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

#pragma endregion

#pragma region Components

private:
    // @private Mesh component
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    // @private 3D Widget component
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    UWidgetComponent* WidgetComponent;

    // @private SphereCollisionComponent
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    USphereComponent* SphereCollision;

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
        {
            FDataInteract* CurrentInteractData = InteractData.DataTable->FindRow<FDataInteract>(InteractData.RowName, "");
            if (CurrentInteractData) return *CurrentInteractData;
        }

        return FDataInteract();
    };


    UFUNCTION(BlueprintPure, Category = "AInteractItemActor | DataInteractItem")
    int32 GetItemCount() const { return ItemCount; }

    UFUNCTION(BlueprintPure, Category = "AInteractItemActor | DataInteractItem")
    ARSInteractStaticItemBase* GetChildStaticActor() const { return ChildStaticItemActor; }

    void SetInteractText(FText NewText);

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Floating Settings",
        meta=(EditCondition = "TypeItem == ETypeItem::InvItem", EditConditionHides, ClampMin = 0, ClampMax = 100))
    float FloatingHeight = 0.0f;

    UPROPERTY(VisibleDefaultsOnly, Category = "Floating Settings")
    FTimeline FloatingTimeline;

private:
    UPROPERTY(EditInstanceOnly, Category = "Settings Interact", DisplayName = "Радиус взаимодействия")
    float CollisionRadius = 100.0f;

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

    // @private InteractText
    UPROPERTY(EditInstanceOnly, Category = "Settings Interact",
        meta = (
            DisplayName = "Использовать уникальный для этого объекта текст взаимодействия",
            ToolTip =
            "Если включено, текст будет браться из соответствующего поля. Если выключено - будет отображаться имя предмета. Приоритет - наивысший."
        )
    )
    bool bCustomInteractText = false;

    // @private InteractText
    UPROPERTY(EditAnywhere, Category = "Settings Interact",
        meta = (
            DisplayName = "Текст взаимодействия",
            ToolTip = "Введите текст, который будет отображаться при взаимодействии",
            EditCondition = "bCustomInteractText",
            EditConditionHides
        )
    )
    FText InteractText;

    UPROPERTY()
    ARSInteractStaticItemBase* ChildStaticItemActor = nullptr;

    UPROPERTY()
    UInteractWidget* InteractWidget;

    FTimerHandle ResetInteractAnimTimerHandle;

    void InitDataInteract(const FDataTableRowHandle NewInteractData, const bool bInitWidgetText = false);

    void MoveItemDown();

    FVector GetItemBounds();


#pragma endregion

#pragma region Statics

public:
    /**
     * Spawns item in front of actor
     * @param Spawner AActor that spawns item
     * @param InventoryItemRules FInventoryItem struct that uses when spawning item
     * @param Count Count of spawning item. By default = 1.
     * @param Distance Distance from Spawner where to spawn item. By default = 150.0f.
     */
    UFUNCTION(BlueprintCallable)
    static void SpawnItem(AActor* Spawner, FInventoryItem InventoryItemRules, int32 Count = 1, float Distance = 200.0f,
        bool RandomDirection = false);


#pragma endregion Statics
};
