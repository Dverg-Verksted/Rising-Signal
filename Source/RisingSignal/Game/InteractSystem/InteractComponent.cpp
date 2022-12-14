// It is owned by the company Dverg Verksted.

#include "Game/InteractSystem/InteractComponent.h"

#include "InteractDataItem.h"
#include "InteractItemActor.h"
#include "RSInteractStaticItemBase.h"
#include "Game/Inventory/RSLootComponent.h"
#include "AnimNotifies/RSItemPickUpEndedAnimNotify.h"
#include "Components/BoxComponent.h"
#include "Game/JournalSystem/JournalSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/RSGamePLayer.h"
#include "Player/RSGamePlayerController.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
    PrimaryComponentTick.bAllowTickOnDedicatedServer = false;
    PrimaryComponentTick.bTickEvenWhenPaused = false;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(FName("Box Collision component"));
    if (BoxCollision)
    {
        BoxCollision->SetBoxExtent(SizeBoxCollision);
    }

    // static ConstructorHelpers::FObjectFinder<UAnimMontage> PickMontage(
    //     TEXT("AnimMontage'/ALS/ALS/RS_Animation/Interact/A_RS_PickupItem_Poses_Montage.A_RS_PickupItem_Poses_Montage'"));
    // GroundPickUpAnimMontage = PickMontage.Object;
    // if (!GroundPickUpAnimMontage)
    // {
    //     LOG_RS(ELogRSVerb::Error, "GroundPickUpAnimMontage didn't set!");
    // }
}

// Called when the game starts
void UInteractComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerPlayer = Cast<ARSBaseCharacter>(GetOwner());
    if (!OwnerPlayer)
    {
        LOG_INTERACT(ELogRSVerb::Error, "Owner player is nullptr");
        return;
    }

    PlayerController = Cast<ARSGamePlayerController>(OwnerPlayer->GetController());
    if (!OwnerPlayer)
    {
        LOG_INTERACT(ELogRSVerb::Error, "Player controller is nullptr");
        return;
    }
    PlayerController->OnInteract.AddDynamic(this, &UInteractComponent::RegisterInteractEvent);

    InventoryComp = OwnerPlayer->FindComponentByClass<URSInventoryComponent>();
    if (!InventoryComp)
    {
        LOG_INTERACT(ELogRSVerb::Error, "Inventory Component is nullptr");
        return;
    }

    JournalSystem = Cast<UJournalSystem>(OwnerPlayer->GetComponentByClass(UJournalSystem::StaticClass()));
    if (!JournalSystem)
    {
        LOG_INTERACT(ELogRSVerb::Error, "Journal System is nullptr");
        return;
    }

    if (!BoxCollision)
    {
        LOG_INTERACT(ELogRSVerb::Error, "Box Collision is nullptr");
        return;
    }

    BoxCollision->AttachToComponent(OwnerPlayer->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::RegisterBeginOverlapInteractItem);
    BoxCollision->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::RegisterEndOverlapInteractItem);
    BoxCollision->SetCollisionProfileName("OverlapAll");

    InitAnimations();
}

void UInteractComponent::Print_Log(ELogRSVerb LogVerb, FString Text, int Line, const char* Function) const
{
    URSFunctionLibrary::Print_Log(LogVerb, FString::Printf(TEXT("Owner player: [%s] | Text: [%s]"), *OwnerPlayer->GetName(), *Text), Line,
        Function);
}

void UInteractComponent::RegisterBeginOverlapInteractItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor) return;
    AInteractItemActor* BaseItem = Cast<AInteractItemActor>(OtherActor);
    URSLootComponent* LootComp = Cast<URSLootComponent>(OtherComp);
    if (!BaseItem && !LootComp) return;

    LOG_INTERACT(ELogRSVerb::Display, FString::Printf(TEXT("Begin overlap actor: [%s]"), *OtherActor->GetName()));
    AddItem(BaseItem);
    AddSearchItem(LootComp);
}

void UInteractComponent::RegisterEndOverlapInteractItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor) return;
    AInteractItemActor* Item = Cast<AInteractItemActor>(OtherActor);
    if (!Item) return;

    LOG_INTERACT(ELogRSVerb::Display, FString::Printf(TEXT("End overlap actor: [%s]"), *OtherActor->GetName()));

    ArrInteractItem.Remove(Item);
    Item->DestroyInteractWidget();
    if (ArrInteractItem.Num() == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(CheckedInteractItemTimerHandle);
        TargetInteractItem = nullptr;
    }
}

void UInteractComponent::AddItem(AInteractItemActor* InteractItem)
{
    if (!InteractItem)
    {
        LOG_INTERACT(ELogRSVerb::Warning, "Interact item is nullptr");
        return;
    }

    ArrInteractItem.AddUnique(InteractItem);
    if (!GetWorld()->GetTimerManager().TimerExists(CheckedInteractItemTimerHandle))
    {
        LOG_INTERACT(ELogRSVerb::Display, FString::Printf(TEXT("Start timer for checked distance | Num item: %i | Rate time start: %f"),
            ArrInteractItem.Num(), RateTimeChecked));
        GetWorld()->GetTimerManager().SetTimer(CheckedInteractItemTimerHandle, this, &UInteractComponent::CheckDistanceToItem,
            RateTimeChecked, true);
    }
}

void UInteractComponent::AddSearchItem(URSLootComponent* LootComponent)
{
    if (!LootComponent)
    {
        LOG_INTERACT(ELogRSVerb::Warning, "Search Actor is nullptr");
        return;
    }

    ArrLootComp.AddUnique(LootComponent);
    if (!GetWorld()->GetTimerManager().TimerExists(CheckedInteractItemTimerHandle))
    {
        LOG_INTERACT(ELogRSVerb::Display, FString::Printf(TEXT("Start timer for checked distance | Num item: %i | Rate time start: %f"),
            ArrInteractItem.Num(), RateTimeChecked));
        GetWorld()->GetTimerManager().SetTimer(CheckedInteractItemTimerHandle, this, &UInteractComponent::CheckDistanceToItem,
            RateTimeChecked, true);
    }
}

void UInteractComponent::RemoveItem(AInteractItemActor* InteractItem)
{
    if (!InteractItem)
    {
        LOG_INTERACT(ELogRSVerb::Warning, "Interact item is nullptr");
        return;
    }

    ArrInteractItem.Remove(InteractItem);
    InteractItem->DestroyInteractWidget();
    if (ArrInteractItem.Num() == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(CheckedInteractItemTimerHandle);
    }
}

void UInteractComponent::CheckDistanceToItem()
{
    bool ItemCloser;
    AInteractItemActor* TempTargetItem = nullptr;
    float TempTargetItemDist = MAX_FLT;
    URSLootComponent* TempTargetSearch = nullptr;
    float TempTargetSearchDist = MAX_FLT;

    if (ArrInteractItem.Num())
    {
        const FVector L_CurrLoc = GetOwner()->GetActorLocation();

        for (const auto L_Item : ArrInteractItem)
        {
            float L_TempDist = (L_Item->GetActorLocation() - L_CurrLoc).Size();
            if (TempTargetItemDist > L_TempDist)
            {
                TempTargetItem = L_Item;
                TempTargetItemDist = L_TempDist;
            }
        }
    }

    if (ArrLootComp.Num())
    {
        const FVector L_CurrLoc = GetOwner()->GetActorLocation();

        for (const auto L_Search : ArrLootComp)
        {
            float L_TempDist = (L_Search->GetOwner()->GetActorLocation() - L_CurrLoc).Size();
            if (TempTargetSearchDist > L_TempDist)
            {
                TempTargetSearch = L_Search;
                TempTargetSearchDist = L_TempDist;
            }
        }
    }

    if (TempTargetItem && TempTargetSearch)
    {
        ItemCloser = TempTargetItemDist >= TempTargetSearchDist;
    }
    else if (TempTargetItem)
    {
        ItemCloser = true;
    }
    else if (TempTargetSearch)
    {
        ItemCloser = false;
    }
    else
    {
        return;
    }

    if (ItemCloser)
    {
        if (TargetInteractItem != TempTargetItem)
        {
            LOG_INTERACT(ELogRSVerb::Display, FString::Printf(TEXT("New target item: %s | Distance: %f"),
                *TempTargetItem->GetName(), TempTargetItemDist));

            if (TargetInteractItem)
            {
                TargetInteractItem->DestroyInteractWidget();
            }

            TargetInteractItem = TempTargetItem;
            TargetInteractItem->LoadInteractWidget();
        }
    }
    else
    {
        if (TargetSearchItem != TempTargetSearch)
        {
            LOG_INTERACT(ELogRSVerb::Display, FString::Printf(TEXT("New target item: %s | Distance: %f"),
                *TempTargetItem->GetName(), TempTargetSearchDist));

            if (TargetSearchItem)
            {
                // TargetSearchItem->DestroyInteractWidget();
            }

            TargetSearchItem = TempTargetSearch;
            // TargetSearchItem->LoadInteractWidget();
        }
    }

}

void UInteractComponent::RegisterInteractEvent()
{
    LOG_INTERACT(ELogRSVerb::Display, "Pressed button interact");

    const ARSBaseCharacter* Player = GetOwner<ARSBaseCharacter>();
    if (!Player || Player->GetCharacterMovement()->IsFalling()) // TODO: Add check for rolling
    {
        LOG_INTERACT(ELogRSVerb::Warning, "Can't interact, Player is Falling");
        return;
    }

    if (TargetSearchItem)
    {
        // TODO: Implement Search Logic
    }

    if (TargetInteractItem)
    {
        FDataTableRowHandle InteractRowHandle = TargetInteractItem->GetInteractData();
        FDataInteract* DataInteract = InteractRowHandle.DataTable->FindRow<FDataInteract>(InteractRowHandle.RowName, "");
        if (!DataInteract) return;

        switch (DataInteract->TypeItem)
        {
            case ETypeItem::NoteItem:
            {
                SendNoteData(DataInteract);
                RemoveItem(TargetInteractItem);
                break;
            }
            case ETypeItem::AudioItem:
            {
                SendAudioData(DataInteract);
                RemoveItem(TargetInteractItem);
                break;
            }
            case ETypeItem::PhotoItem:
            {
                SendPhotoData(DataInteract);
                RemoveItem(TargetInteractItem);
                break;
            }
            case ETypeItem::InvItem:
            {
                if (DataInteract->RowRuleInvItem.DataTable && InteractRowHandle.RowName != "None")
                {
                    InventoryComp->AddDataItem(DataInteract->RowRuleInvItem, InteractRowHandle.RowName, TargetInteractItem->GetItemCount());
                    RemoveItem(TargetInteractItem);
                }
                break;
            }
            case ETypeItem::StaticItem:
            {
                if (ARSInteractStaticItemBase* ChildStaticActor = TargetInteractItem->GetChildStaticActor())
                {
                    ChildStaticActor->Interact(OwnerPlayer);
                }

                return;
            }
            default:
            {
            }
        }

        StartPickUpAnimation();
        TargetInteractItem->Destroy();
    }
}

void UInteractComponent::SendNoteData(const FDataInteract* DataInteract) const
{
    if (!JournalSystem)
    {
        return;
    }

    FInteractItemNote InteractItemNote;
    InteractItemNote.NoteDate = DataInteract->NoteDate;
    InteractItemNote.NoteDescription = DataInteract->NoteDescription;
    InteractItemNote.NoteHeader = DataInteract->NoteHeader;
    InteractItemNote.NoteMap = DataInteract->AttachedMap;
    JournalSystem->AddNoteItem(InteractItemNote);
}

void UInteractComponent::SendAudioData(const FDataInteract* DataInteract) const
{
    FInteractItemAudio InteractItemAudio;
    InteractItemAudio.AudioHeader = DataInteract->AudioHeader;
    InteractItemAudio.AudioMap = DataInteract->AttachedMap;
    InteractItemAudio.JournalAudio = DataInteract->JournalAudio;
    JournalSystem->AddAudioItem(InteractItemAudio);
}

void UInteractComponent::SendPhotoData(const FDataInteract* DataInteract) const
{
    FInteractItemPhoto InteractItemPhoto;
    InteractItemPhoto.PhotoHeader = DataInteract->PhotoHeader;
    InteractItemPhoto.PhotoMap = DataInteract->AttachedMap;
    InteractItemPhoto.JournalPhoto = DataInteract->JournalPhoto;
    JournalSystem->AddPhotoItem(InteractItemPhoto);
}

void UInteractComponent::InitAnimations()
{
    if (GroundPickUpAnimMontage)
    {
        const auto NotifyEvents = GroundPickUpAnimMontage->Notifies;
        for (const auto NotifyEvent : NotifyEvents)
        {
            const auto PickUpEndedNotify = Cast<URSItemPickUpEndedAnimNotify>(NotifyEvent.Notify);
            if (PickUpEndedNotify)
            {
                PickUpEndedNotify->OnNotifiedInteract.AddUObject(this, &UInteractComponent::PickUpAnimationEnded);
                break;
            }
        }
    }
    else
    {
        LOG_INTERACT(ELogRSVerb::Error, "No PickUpAnimMontage Set in InteractComponent!");
    }
}

void UInteractComponent::StartPickUpAnimation() const
{
    if (!OwnerPlayer || !PlayerController)
    {
        LOG_INTERACT(ELogRSVerb::Error, "Owner player or PlayerController is nullptr");
        return;
    }

    OwnerPlayer->GetCharacterMovement()->DisableMovement();

    OwnerPlayer->PlayAnimMontage(GroundPickUpAnimMontage);

    // some workaround
    // FTimerHandle TempHandle;
    //
    // GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &UInteractComponent::PickUpAnimationEnded, 2); // TODO: Fix this workaround
}

void UInteractComponent::PickUpAnimationEnded() const
{
    if (!OwnerPlayer || !PlayerController)
    {
        LOG_INTERACT(ELogRSVerb::Error, "Owner player or PlayerController is nullptr");
        return;
    }

    OwnerPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

#if WITH_EDITOR
void UInteractComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (PropertyChangedEvent.Property && PropertyChangedEvent.MemberProperty->GetName() == TEXT("SizeBoxCollision"))
    {
        if (BoxCollision)
        {
            BoxCollision->SetBoxExtent(SizeBoxCollision);
        }
    }
}
#endif
