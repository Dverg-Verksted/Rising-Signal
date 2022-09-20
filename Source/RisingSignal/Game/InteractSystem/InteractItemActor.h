// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "InteractDataItem.h"
#include "Engine/DataTable.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "GameFramework/Actor.h"
#include "InteractItemActor.generated.h"


class USphereComponent;
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

    virtual void PostLoad() override;

#if UE_EDITOR

    // Called when changed property
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

#pragma endregion

#pragma region Components

    UPROPERTY(EditInstanceOnly, Category = "Components")
    float CollisionRadius = 100.0f;

private:
    // @private Mesh component
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* Mesh;

    // @private 3D Widget component
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UWidgetComponent* WidgetComponent;

    // @private SphereCollisionComponent
    UPROPERTY(VisibleAnywhere, Category = "Components")
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

#pragma endregion

#pragma region Statics
public:
    UFUNCTION(BlueprintCallable)
    static void SpawnItem(AActor* Spawner, FInventoryItem InventoryItemRules, int32 Count = 1, float Distance = 150.0f);

    static FName GetRowNameByItemName(const UDataTable* DataTable, FText ItemName);

#pragma endregion
};
