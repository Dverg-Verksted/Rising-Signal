// It is owned by the company Dverg Verksted.

#include "Game/InteractSystem/InteractComponent.h"
#include "InteractItemActor.h"
#include "Components/BoxComponent.h"
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
}

// Called when the game starts
void UInteractComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerPlayer = Cast<ARSGamePLayer>(GetOwner());
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

    if (!BoxCollision)
    {
        LOG_INTERACT(ELogRSVerb::Error, "Box Collision is nullptr");
        return;
    }
    
    BoxCollision->AttachToComponent(OwnerPlayer->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::RegisterBeginOverlapInteractItem);
    BoxCollision->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::RegisterEndOverlapInteractItem);
    BoxCollision->SetCollisionProfileName("OverlapAll");
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
    AInteractItemActor* Item = Cast<AInteractItemActor>(OtherActor);
    if (!Item) return;

    LOG_INTERACT(ELogRSVerb::Display, FString::Printf(TEXT("Begin overlap actor: [%s]"), *OtherActor->GetName()));

    ArrInteractItem.AddUnique(Item);
    if (!GetWorld()->GetTimerManager().TimerExists(CheckedInteractItemTimerHandle))
    {
        LOG_INTERACT(ELogRSVerb::Display, FString::Printf(TEXT("Start timer for checked distance | Num item: %i | Rate time start: %f"),
            ArrInteractItem.Num(), RateTimeChecked));
        GetWorld()->GetTimerManager().SetTimer(CheckedInteractItemTimerHandle, this, &UInteractComponent::CheckDistanceToItem,
            RateTimeChecked, true);
    }
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

void UInteractComponent::CheckDistanceToItem()
{
    if (ArrInteractItem.Num() == 0) return;

    const FVector L_CurrLoc = GetOwner()->GetActorLocation();
    AInteractItemActor* TempTargetItem = ArrInteractItem[0];
    float L_MinDistance = (TempTargetItem->GetActorLocation() - L_CurrLoc).Size();

    for (const auto L_Item : ArrInteractItem)
    {
        float L_TempDist = (L_Item->GetActorLocation() - L_CurrLoc).Size();
        if (L_MinDistance > L_TempDist)
        {
            TempTargetItem = L_Item;
            L_MinDistance = L_TempDist;
        }
    }

    if (TargetInteractItem != TempTargetItem)
    {
        LOG_INTERACT(ELogRSVerb::Display, FString::Printf(TEXT("New target item: %s | Distance: %f"),
            *TempTargetItem->GetName(), L_MinDistance));
        
        if (TargetInteractItem)
        {
            TargetInteractItem->DestroyInteractWidget();
        }
        
        TargetInteractItem = TempTargetItem;
        TargetInteractItem->LoadInteractWidget();
    }
}

void UInteractComponent::RegisterInteractEvent()
{
    LOG_INTERACT(ELogRSVerb::Display, "Pressed button interact");
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
