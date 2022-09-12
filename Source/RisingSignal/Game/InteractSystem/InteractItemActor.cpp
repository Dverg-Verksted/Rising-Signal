// It is owned by the company Dverg Verksted.

#include "Game/InteractSystem/InteractItemActor.h"

// #include "RSInteractStaticItemBase.h"
#include "Components/WidgetComponent.h"
// #include "Engine/AssetManager.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "Library/RSFunctionLibrary.h"
#include "Widgets/InteractWidget.h"

// Sets default values
AInteractItemActor::AInteractItemActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
    PrimaryActorTick.bAllowTickOnDedicatedServer = false;
    PrimaryActorTick.bTickEvenWhenPaused = false;

    this->Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh component"));
    SetRootComponent(this->Mesh);

    static ConstructorHelpers::FClassFinder<UInteractWidget> UnitSelector(
        TEXT("/Game/RisingSignal/Core/HUD/UI_UserHUD/Widgets/WBP_InteractText"));
    SubInteractWidget = UnitSelector.Class;

    this->WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("3D Widget component"));
    this->WidgetComponent->SetupAttachment(this->Mesh);
    this->WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    this->WidgetComponent->SetDrawAtDesiredSize(true);
    this->WidgetComponent->SetWidgetClass(SubInteractWidget);

    static ConstructorHelpers::FObjectFinder<UDataTable> InteractItemDT(
        TEXT("DataTable'/Game/RisingSignal/Core/InteractItems/DT_DataInteract.DT_DataInteract'"));
    DataTableInteractItem = InteractItemDT.Object;
    if (DataTableInteractItem)
    {
        InteractData.DataTable = DataTableInteractItem;
    }

    // LOG_RS(ELogRSVerb::Warning, "Constructed " + GetName());
}

// Called when the game starts or when spawned
void AInteractItemActor::BeginPlay()
{
    Super::BeginPlay();

    InteractWidget = Cast<UInteractWidget>(this->WidgetComponent->GetWidget());
    if (IsValid(InteractWidget))
    {
        InteractWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        LOG_RS(ELogRSVerb::Error, GetName() + "Has no InteractWidget");
    }

    InitDataInteract(InteractData);
}

#if UE_EDITOR

void AInteractItemActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (!PropertyChangedEvent.Property)
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Interact actor: %s have property is nullptr"), *GetName()));
        return;
    }

    // LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("Name changed property: %s"), *PropertyChangedEvent.Property->GetName()));

    if (PropertyChangedEvent.Property->GetName() == TEXT("RowName"))
    {
        FDataInteract* DataInteract = InteractData.DataTable->FindRow<FDataInteract>(InteractData.RowName, "");
        if (!DataInteract) return;

        if (DataInteract->TypeItem == ETypeItem::StaticItem)
        {
            this->Mesh->SetStaticMesh(nullptr);

            // FTransform StaticItemActorTransform{GetActorRotation(), GetActorLocation()};

            // ChildStaticItemActor = GetWorld()->SpawnActor<ARSInteractStaticItemBase>(DataInteract->StaticActorClass, StaticItemActorTransform);
            //
            // // ChildStaticItemActor = Cast<ARSInteractStaticItemBase>(GEditor->AddActor(GetWorld()->GetCurrentLevel(),
            // //     DataInteract->StaticActorClass,
            // //     StaticItemActorTransform));
            // if (ChildStaticItemActor)
            // {
            //     LOG_RS(ELogRSVerb::Display, ChildStaticItemActor->GetName() + " parenting to " + this->GetName());
            //     // GEditor->ParentActors(this, ChildStaticItemActor, "");
            //     ChildStaticItemActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
            //     LOG_RS(ELogRSVerb::Display, ChildStaticItemActor->GetName() + " successfully parent to " + this->GetName());
            // }
        }
        else
        {
            // if (ChildStaticItemActor)
            // {
            //     LOG_RS(ELogRSVerb::Display, ChildStaticItemActor->GetName() + " start destroing");
            //     ChildStaticItemActor->Destroy();
            //     // GWorld->DestroyActor(ChildStaticItemActor);
            //
            //     LOG_RS(ELogRSVerb::Display, ChildStaticItemActor->GetName() + " destroyed");
            // }
            UStaticMesh* L_Mesh = LoadObject<UStaticMesh>(nullptr, *(DataInteract->MeshItem.ToString()));
            if (L_Mesh)
            {
                this->Mesh->SetStaticMesh(L_Mesh);
            }
            else
            {
                LOG_RS(ELogRSVerb::Error, "StaticMesh for " + this->GetName() + " is nullptr");
            }
        }

        this->TypeItem = DataInteract->TypeItem;
        this->NameItem = DataInteract->Name;
        this->DescriptionItem = DataInteract->DescriptionItem;
    }
}

#endif

void AInteractItemActor::LoadInteractWidget()
{
    if (InteractWidget)
    {
        // LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("Name interact actor: %s | Load interact Widget"), *GetName()));
        GetWorldTimerManager().ClearTimer(ResetInteractAnimTimerHandle);
        InteractWidget->SetVisibility(ESlateVisibility::Visible);
        this->InteractWidget->StartAnimation();
    }
}

void AInteractItemActor::DestroyInteractWidget()
{
    if (InteractWidget)
    {
        // LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("Name interact actor: %s | Destroy interact Widget"), *GetName()));
        InteractWidget->EndAnimation();
        GetWorldTimerManager().SetTimer(ResetInteractAnimTimerHandle, [&]()
        {
            InteractWidget->SetVisibility(ESlateVisibility::Collapsed);
        }, InteractWidget->GetEndAnim()->GetEndTime(), false);
    }
}

void AInteractItemActor::InitDataInteract(FDataTableRowHandle NewInteractData)
{
    if (NewInteractData.DataTable && NewInteractData.RowName != "")
    {
        const FDataInteract* DataInteract = NewInteractData.DataTable->FindRow<FDataInteract>(NewInteractData.RowName, "");
        if (!DataInteract) return;

        if (DataInteract->MeshItem.IsNull())
        {
            this->Mesh->SetStaticMesh(nullptr);
        }
        else
        {
            UStaticMesh* L_Mesh = LoadObject<UStaticMesh>(nullptr, *(DataInteract->MeshItem.ToString()));
            if (!L_Mesh) return;
            this->Mesh->SetStaticMesh(L_Mesh);
        }

        this->TypeItem = DataInteract->TypeItem;

        if (DataInteract->TypeItem == ETypeItem::InvItem &&
            DataInteract->RowRuleInvItem.DataTable &&
            DataInteract->RowRuleInvItem.RowName != "")
        {
            const FInventoryItem* DataInventory = DataInteract->RowRuleInvItem.DataTable->FindRow<FInventoryItem>(
                DataInteract->RowRuleInvItem.RowName, "");
            if (DataInventory)
            {
                this->NameItem = DataInventory->Name;
                this->DescriptionItem = DataInventory->Description;
            }
            else
            {
                LOG_RS(ELogRSVerb::Error, "Inventory RowHandle didn't set for " + GetName());
            }
        }
        else
        {
            this->NameItem = DataInteract->Name;
            this->DescriptionItem = DataInteract->DescriptionItem;
        }

        if (this->bCustomInteractText)
        {
            InteractWidget->SetText(this->InteractText);
        }
        else if (DataInteract->bCustomInteractText)
        {
            InteractWidget->SetText(DataInteract->InteractText);
        }
        else
        {
            InteractWidget->SetText(this->NameItem);
        }
    }
}

void AInteractItemActor::SpawnItem(AActor* Spawner, FInventoryItem InventoryItemRules, int32 Count, float Distance)
{
    if (!Spawner) return;

    if (Count < 1) return;

    if (Distance < 1) return;

    FVector SpawnerLocation = Spawner->GetActorLocation();
    FVector SpawnerRotation = Spawner->GetActorForwardVector();

    FVector ItemSpawnLocation = SpawnerLocation + (SpawnerRotation * Distance);

    FTransform ItemSpawnTransform{ItemSpawnLocation};

    AInteractItemActor* Item = Spawner->GetWorld()->SpawnActor<AInteractItemActor>(AInteractItemActor::StaticClass(), ItemSpawnTransform);
    if (Item && Item->InteractData.DataTable)
    {
        Item->TypeItem = ETypeItem::InvItem;
        FDataTableRowHandle NewDTRowHandle;
        NewDTRowHandle.DataTable = Item->InteractData.DataTable;
        NewDTRowHandle.RowName = InventoryItemRules.InteractRowName;
        Item->InitDataInteract(NewDTRowHandle);
        Item->InteractData.RowName = InventoryItemRules.InteractRowName;
        Item->ItemCount = Count;
    }
    // AInteractItemActor* Item = Spawner->GetWorld()->SpawnActorDeferred<AInteractItemActor>(AInteractItemActor::StaticClass(),
    //     ItemSpawnTransform);
    // if (Item && Item->InteractData.DataTable)
    // {
    //     Item->TypeItem = ETypeItem::InvItem;
    //     Item->InteractData.RowName = GetRowNameByItemName(Item->InteractData.DataTable, (InventoryItemRules.Name));
    //     Item->ItemCount = Count;
    //
    //     Item->FinishSpawning(ItemSpawnTransform);
    // }
}


FName AInteractItemActor::GetRowNameByItemName(const UDataTable* DataTable, FText ItemName)
{
    if (DataTable)
    {
        TMap<FName, uint8*> DTMap = DataTable->GetRowMap();

        for (auto Pair : DTMap)
        {
            FDataInteract* ItemStruct = (FDataInteract*)Pair.Value;

            if (ItemStruct->Name.EqualTo(ItemName))
                return Pair.Key;
        }
    }

    return "None";
}
