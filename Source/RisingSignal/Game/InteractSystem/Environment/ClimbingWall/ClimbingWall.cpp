// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/Environment/ClimbingWall/ClimbingWall.h"

#include "Components/BoxComponent.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"

AClimbingWall::AClimbingWall()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
    InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
    InteractionVolume->SetupAttachment(RootComponent);
    InteractionVolume->SetCollisionProfileName("PawnInteraction");
    InteractionVolume->SetGenerateOverlapEvents(true);

    Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
    Wall->SetupAttachment(RootComponent);
    Wall->SetRelativeScale3D(FVector(0.2f, 1.0f, 1.0f));
}

void AClimbingWall::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    Wall->SetRelativeLocation(FVector(0.0f, -WallWidth * 0.5f, WallLength * 0.5f));

    UStaticMesh* WallMesh = Wall->GetStaticMesh();
    if(IsValid(WallMesh))
    {
        float MeshHeight = WallMesh->GetBoundingBox().GetSize().Z;
        float MeshWidth = WallMesh->GetBoundingBox().GetSize().Y;
        if(!FMath::IsNearlyZero(MeshHeight))
        {
            FVector WallScale = Wall->GetRelativeScale3D();
            Wall->SetRelativeScale3D(FVector(WallScale.X, WallWidth / MeshWidth, WallLength / MeshHeight));
            FVector WallLocation = Wall->GetRelativeLocation();
            FVector InteractionLocation(WallLocation.X + 20.0f, WallLocation.Y, WallLocation.Z);
            GetInteractionBox()->SetRelativeLocation(InteractionLocation);
            GetInteractionBox()->SetBoxExtent(FVector(10.0f, WallWidth * 0.5f, WallLength * 0.5f));
        }
    }
}

void AClimbingWall::BeginPlay()
{
    Super::BeginPlay();
}

void AClimbingWall::OnInteractionVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnInteractionVolumeStartOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if(!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
    {
        return;
    }

    ARSBaseCharacter* BaseCharacter = Cast<ARSBaseCharacter>(OtherActor);
    if(BaseCharacter)
    {
        BaseCharacter->InteractWithWall();
    }
}

void AClimbingWall::OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnInteractionVolumeEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
