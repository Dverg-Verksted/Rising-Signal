// It is owned by the company Dverg Verksted.

#include "Game/InteractSystem/InteractItemActor.h"
#include "RSInteractStaticItemBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"
#include "Widgets/InteractWidget.h"

// Sets default values
AInteractItemActor::AInteractItemActor()
{
    // // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    SetActorTickEnabled(true);
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
    // PrimaryActorTick.bAllowTickOnDedicatedServer = false;
    // PrimaryActorTick.bTickEvenWhenPaused = false;

    this->Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh component"));
    SetRootComponent(this->Mesh);
    Mesh->SetSimulatePhysics(false);

    static ConstructorHelpers::FClassFinder<UInteractWidget> UnitSelector(
        TEXT("/Game/RisingSignal/Core/HUD/UI_UserHUD/Widgets/WBP_InteractText"));
    SubInteractWidget = UnitSelector.Class;

    this->WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("3D Widget component"));
    this->WidgetComponent->SetupAttachment(this->Mesh);
    this->WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    this->WidgetComponent->SetDrawAtDesiredSize(true);
    this->WidgetComponent->SetWidgetClass(SubInteractWidget);

    this->SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Item Collision"));
    this->SphereCollision->SetupAttachment(RootComponent);
    this->SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollision->SetSphereRadius(CollisionRadius);

    static ConstructorHelpers::FObjectFinder<UDataTable> InteractItemDT(
        TEXT("DataTable'/Game/RisingSignal/Core/InteractItems/DT_DataInteract.DT_DataInteract'"));
    DataTableInteractItem = InteractItemDT.Object;
    if (DataTableInteractItem)
    {
        InteractData.DataTable = DataTableInteractItem;
    }

    static ConstructorHelpers::FObjectFinder<UCurveFloat> FloatCurve(
        TEXT("CurveFloat'/Game/RisingSignal/Core/InteractItems/CF_FloatingItem.CF_FloatingItem'"));
    FloatingCurve = FloatCurve.Object;

}


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

    InitDataInteract(InteractData, true);

    if (TypeItem != ETypeItem::StaticItem)
        MoveItemDown();

    if (FloatingCurve)
    {
        FOnTimelineFloat TimelineProgress;
        TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
        FloatingTimeline.AddInterpFloat(FloatingCurve, TimelineProgress);
        FloatingTimeline.SetPlayRate(FloatingSpeed);
        FloatingTimeline.SetLooping(true);
        FloatingTimeline.SetNewTime(UKismetMathLibrary::RandomFloatInRange(0.0f, 4.0f));
    }

    if (ShouldFloat)
    {
        StartFloating(true);
    }
}

void AInteractItemActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FloatingTimeline.TickTimeline(DeltaSeconds);

}

void AInteractItemActor::PostLoad()
{
    Super::PostLoad();
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

    if (PropertyChangedEvent.Property->GetName() == TEXT("CollisionRadius"))
    {
        SphereCollision->SetSphereRadius(CollisionRadius);
    }

    if (PropertyChangedEvent.Property->GetName() == TEXT("RowName"))
    {
        if (ChildStaticItemActor)
        {
            ChildStaticItemActor->Destroy();
            ChildStaticItemActor = nullptr;
        }

        InitDataInteract(InteractData);
    }
}

#endif

void AInteractItemActor::LoadInteractWidget()
{
    if (InteractWidget)
    {
        GetWorldTimerManager().ClearTimer(ResetInteractAnimTimerHandle);
        InteractWidget->SetVisibility(ESlateVisibility::Visible);
        this->InteractWidget->StartAnimation();
    }
}

void AInteractItemActor::DestroyInteractWidget()
{
    if (InteractWidget)
    {
        InteractWidget->EndAnimation();
        // GetWorldTimerManager().SetTimer(ResetInteractAnimTimerHandle, [&]() //TODO: Check it
        // {
        //     if (InteractWidget)
        //         InteractWidget->SetVisibility(ESlateVisibility::Collapsed);
        // }, InteractWidget->GetEndAnim()->GetEndTime(), false);
    }
}

void AInteractItemActor::SetInteractText(FText NewText)
{
    if (InteractWidget)
        InteractWidget->SetText(NewText);
}

void AInteractItemActor::StartFloating(bool bStart)
{

    if (!ShouldFloat) return;

    if (bStart)
    {
        FloatingTimeline.Play();
    }
    else
    {
        FloatingTimeline.Stop();
    }
}

void AInteractItemActor::TimelineProgress(float Value)
{

    if (SavedLocation != FVector::ZeroVector)
    {
        FVector NewLoc = SavedLocation;

        NewLoc.Z += Value * FloatingHeight;

        SetActorLocation(NewLoc);
    }
}

void AInteractItemActor::InitDataInteract(const FDataTableRowHandle NewInteractData, const bool bInitWidgetText)
{
    if (NewInteractData.DataTable && NewInteractData.RowName != "None")
    {
        const FDataInteract* DataInteract = NewInteractData.DataTable->FindRow<FDataInteract>(NewInteractData.RowName, "");
        if (!DataInteract) return;

#if WITH_EDITOR

        // LOG_RS(ELogRSVerb::Warning, GetWorld()->GetName() + " = CurrentMapName --- SelectedMapName = " + DataInteract->AttachedMap.GetAssetName());

        if (!DataInteract->AttachedMap.IsNull() && DataInteract->AttachedMap.GetAssetName() != GetWorld()->GetName())
        {
            this->Mesh->SetStaticMesh(nullptr);
            LOG_RS(ELogRSVerb::Error, "Selected interact Note/Audio/Photo can't be placed on this Map");
            return;
        }
#endif

        if (DataInteract->TypeItem == ETypeItem::StaticItem)
        {
            this->Mesh->SetStaticMesh(nullptr);

            if (!ChildStaticItemActor)
            {
                FTransform StaticItemActorTransform{GetActorRotation(), GetActorLocation()};

                UClass* StaticActorClass = LoadClass<ARSInteractStaticItemBase>(nullptr,
                    *DataInteract->StaticActorClassPtr.ToString());

                ChildStaticItemActor = GetWorld()->SpawnActorDeferred<ARSInteractStaticItemBase>(StaticActorClass,
                    StaticItemActorTransform);

                if (ChildStaticItemActor)
                {
                    ChildStaticItemActor->SetOwner(this);

                    ChildStaticItemActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

                    ChildStaticItemActor->FinishSpawning(StaticItemActorTransform);
                }
            }
        }
        else
        {
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

        if (DataInteract->TypeItem == ETypeItem::InvItem &&
            DataInteract->RowRuleInvItem.DataTable &&
            DataInteract->RowRuleInvItem.RowName != "None")
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
            this->ShouldFloat = false;
            this->NameItem = DataInteract->Name;
            this->DescriptionItem = DataInteract->DescriptionItem;
        }

        if (bInitWidgetText)
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

void AInteractItemActor::MoveItemDown()
{

    LOG_RS(ELogRSVerb::Warning, GetName() + " " + FString::SanitizeFloat(GetItemZBound()));

    FHitResult Hit;
    GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), FVector::DownVector * 2000, ECC_WorldStatic);

    if (Hit.bBlockingHit)
    {
        FVector NewLoc = GetActorLocation();
        NewLoc.Z = Hit.ImpactPoint.Z + GetItemZBound() / 2 + FloatingHeight + UKismetMathLibrary::RandomFloatInRange(0.0f, 30.0f);

        SetActorLocation(NewLoc);
    }

    SavedLocation = GetActorLocation();
}

float AInteractItemActor::GetItemZBound()
{
    if (Mesh)
    {
        FVector Min, Max;
        Mesh->GetLocalBounds(Min, Max);

        return UKismetMathLibrary::Abs(Min.Z) > UKismetMathLibrary::Abs(Max.Z)
                   ? UKismetMathLibrary::Abs(Min.Z)
                   : UKismetMathLibrary::Abs(Max.Z);
    }
    return 0.0f;
}

void AInteractItemActor::SpawnItem(AActor* Spawner, FInventoryItem InventoryItemRules, int32 Count, float Distance, bool RandomDirection)
{
    if (!Spawner) return;

    if (Count < 1) return;

    if (Distance < 1) return;

    FVector SpawnerLocation = Spawner->GetActorLocation();
    FVector SpawnDirection;
    float SpawnDistance = Distance < 150.0f ? 150.0f : Distance;
    SpawnDistance = UKismetMathLibrary::RandomFloatInRange(Distance - 10, Distance + 10);

    if (RandomDirection)
    {
        SpawnDirection = UKismetMathLibrary::RandomUnitVector();
        SpawnDirection.Z = 0;
    }
    else
    {
        SpawnDirection = Spawner->GetActorForwardVector();
    }

    FVector ItemSpawnLocation = SpawnerLocation + (SpawnDirection * SpawnDistance);

    if (ARSBaseCharacter* RSChar = Cast<ARSBaseCharacter>(Spawner))
    {
        float CharHeight = RSChar->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        ItemSpawnLocation.Z -= CharHeight;
    }

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

        if (Item->TypeItem != ETypeItem::StaticItem)
            Item->MoveItemDown();

        Item->ShouldFloat = true;
    }

}
