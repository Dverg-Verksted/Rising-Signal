// It is owned by the company Dverg Verksted.


#include "Game/InteractSystem/Environment/Rope/Rope.h"
#include "Components/BoxComponent.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"

ARope::ARope()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));

    CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
    CableComponent->SetupAttachment(RootComponent);

    CableEndMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CableEndMeshComponent"));
    CableEndMeshComponent->SetupAttachment(RootComponent);

    InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
    InteractionVolume->SetupAttachment(CableEndMeshComponent);
    InteractionVolume->SetCollisionProfileName("PawnInteraction");
    InteractionVolume->SetGenerateOverlapEvents(true);

    EndCable = CreateDefaultSubobject<UCableComponent>(TEXT("EndCableComponent"));
    EndCable->SetupAttachment(CableEndMeshComponent);

    PhysicsConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintComponent"));
    PhysicsConstraintComponent->SetupAttachment(RootComponent);

    
}

void ARope::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if(CableEndMeshComponent)
    {
        CableEndMeshComponent->SetRelativeLocation(FVector(CableEndMeshComponent->GetRelativeLocation().X, CableEndMeshComponent->GetRelativeLocation().Y, -CableLength));
        InteractionVolume->SetRelativeLocation(FVector::ZeroVector);

        FVector InteractionVolumeLocation = GetInteractionBox()->GetRelativeLocation();
        FVector InteractionVolumeScale = GetInteractionBox()->GetUnscaledBoxExtent();

        GetInteractionBox()->SetRelativeLocation(FVector(InteractionVolumeLocation.X, InteractionVolumeLocation.Y, CableLength * 0.5f));
        GetInteractionBox()->SetBoxExtent(FVector(InteractionVolumeScale.X, InteractionVolumeScale.Y, CableLength * 0.5f));
    }
}

void ARope::AddSwingForce(FVector& Force, bool bForce)
{
    float CurrentSwing = GetActorLocation().Z - CableEndMeshComponent->GetComponentLocation().Z;

    if(CurrentSwing > SwingLimit || bForce)
    {
        if(InertionLR)
        {
            CableEndMeshComponent->AddForce(Force * SwingMultiplier, NAME_None, true);
        }
        else
        {
            CableEndMeshComponent->AddForce(Force * -1 * SwingMultiplier, NAME_None, true);
        }
    }
}

void ARope::BeginPlay()
{
    Super::BeginPlay();
}

void ARope::OnInteractionVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnInteractionVolumeStartOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if(!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
    {
        return;
    }

    ARSBaseCharacter* BaseCharacter = Cast<ARSBaseCharacter>(OtherActor);
    if(BaseCharacter)
    {
        FVector CharacterVelocity = BaseCharacter->GetActorForwardVector() * (BaseCharacter->GetBaseCharacterMovementComponent()->Velocity.Size() / 4.0f);
        AddSwingForce(CharacterVelocity, true);
        BaseCharacter->InteractWithRope(this);
    }
}

void ARope::OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    Super::OnInteractionVolumeEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
