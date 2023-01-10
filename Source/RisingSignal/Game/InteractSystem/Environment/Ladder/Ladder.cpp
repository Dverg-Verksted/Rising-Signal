// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/Environment/Ladder/Ladder.h"
#include "Components/BoxComponent.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"

ALadder::ALadder()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LadderRoot"));
	
    LeftRailMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftRail"));
    LeftRailMeshComponent->SetupAttachment(RootComponent);
	
    RightRailMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightRail"));
    RightRailMeshComponent->SetupAttachment(RootComponent);
	
    StepsMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Steps"));
    StepsMeshComponent->SetupAttachment(RootComponent);

    InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
    InteractionVolume->SetupAttachment(RootComponent);
    InteractionVolume->SetCollisionProfileName(FName("PawnInteraction"));
    InteractionVolume->SetGenerateOverlapEvents(true);

    TopInteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TopInteractionVolume"));
    TopInteractionVolume->SetupAttachment(RootComponent);
    TopInteractionVolume->SetCollisionProfileName(FName("PawnInteraction"));
    TopInteractionVolume->SetGenerateOverlapEvents(true);
}

void ALadder::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    LeftRailMeshComponent->SetRelativeLocation(FVector(0.0f, -LadderWeight * 0.5f, LadderHeight * 0.5f));
    RightRailMeshComponent->SetRelativeLocation(FVector(0.0f, LadderWeight * 0.5f, LadderHeight * 0.5f));

    AttachFromTopStartPosition = FVector(AttachFromTopStartPosition.X, AttachFromTopStartPosition.Y, LadderHeight + OffsetZFromTopStartPosition);

    UStaticMesh* LeftRailMesh = LeftRailMeshComponent->GetStaticMesh();
    if(IsValid(LeftRailMesh))
    {
        float MeshHeight = LeftRailMesh->GetBoundingBox().GetSize().Z;
        if(!FMath::IsNearlyZero(MeshHeight))
        {
            LeftRailMeshComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, LadderHeight / MeshHeight + 0.7f));
        }
    }

    UStaticMesh* RightRailMesh = RightRailMeshComponent->GetStaticMesh();
    if(IsValid(RightRailMesh))
    {
        float MeshHeight = RightRailMesh->GetBoundingBox().GetSize().Z;
        if(!FMath::IsNearlyZero(MeshHeight))
        {
            RightRailMeshComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, LadderHeight / MeshHeight + 0.7f));
        }
    }

    UStaticMesh* StepsMesh = StepsMeshComponent->GetStaticMesh();
    if(IsValid(StepsMesh))
    {
        float MeshWidth = StepsMesh->GetBoundingBox().GetSize().Y;
        if(!FMath::IsNearlyZero(MeshWidth))
        {
            StepsMeshComponent->SetRelativeScale3D(FVector(1.0f, LadderWeight / MeshWidth, 1.0f));
        }
    }

    StepsMeshComponent->ClearInstances();

    uint32 StepsCount = FMath::FloorToInt((LadderHeight - BottomStepOffset) / StepsInterval);
    for(uint32 i = 0; i < StepsCount; ++i)
    {
        FTransform InstanceTransform(FVector(0.0f, 0.0f, BottomStepOffset + i * StepsInterval));
        StepsMeshComponent->AddInstance(InstanceTransform);
    }

    float BoxDeathExtent = GetMeshInteractiveBox()->GetUnscaledBoxExtent().X;
    GetMeshInteractiveBox()->SetBoxExtent(FVector(BoxDeathExtent, LadderWeight * 0.5f, LadderHeight * 0.5f));
    GetMeshInteractiveBox()->SetRelativeLocation(FVector(BoxDeathExtent, 0.0f, LadderHeight * 0.5f));

    FVector TopBoxExtent = TopInteractionVolume->GetUnscaledBoxExtent();
    TopInteractionVolume->SetBoxExtent(FVector(TopBoxExtent.X, LadderWeight * 0.5f, TopBoxExtent.Z));
    TopInteractionVolume->SetRelativeLocation(FVector(-TopBoxExtent.X, 0.0f, LadderHeight + TopBoxExtent.Z));
}

void ALadder::BeginPlay()
{
    Super::BeginPlay();

    TopInteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnInteractionVolumeStartOverlap);
    TopInteractionVolume->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnInteractionVolumeEndOverlap);
}

FVector ALadder::GetAttachFromTopEndPosition() const
{
    FRotator OrientationRotation = GetActorForwardVector().ToOrientationRotator();
    FVector Offset = OrientationRotation.RotateVector(OffsetAttachFromTopEndPosition);
    FVector LadderTop = GetActorLocation() + GetActorUpVector() * LadderHeight;
    return LadderTop + Offset;
}

FVector ALadder::GetTopPosition() const
{
    FRotator OrientationRotation = GetActorForwardVector().ToOrientationRotator();
    FVector Offset = OrientationRotation.RotateVector(AttachFromTopStartPosition);
    Offset.Z = OffsetZFromTopStartPosition;
    Offset.X = -10.0f;
    FVector LadderTop = GetActorLocation() + GetActorUpVector() * LadderHeight;
    return LadderTop + Offset;
}

void ALadder::OnInteractionVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnInteractionVolumeStartOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if(!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
    {
        return;
    }
    
    if(OverlappedComponent == TopInteractionVolume)
    {
        bIsOnTop = true;
    }

    ARSBaseCharacter* BaseCharacter = Cast<ARSBaseCharacter>(OtherActor);
    if(!BaseCharacter->GetIsMantling() && !BaseCharacter->GetBaseCharacterMovementComponent()->bIsAttachingToLadder)
    {
        BaseCharacter->InteractWithLadder();
    }
	
}

void ALadder::OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    Super::OnInteractionVolumeEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

    if(!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
    {
        return;
    }
	
    if(OverlappedComponent)
    {
        bIsOnTop = false;
    }
}

UBoxComponent* ALadder::GetMeshInteractiveBox()
{
    return StaticCast<UBoxComponent*>(InteractionVolume);
}
