// It is owned by the company Dverg Verksted.


#include "RSAbilityInteractVolume.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/RSGamePLayer.h"

// Sets default values
ARSAbilityInteractVolume::ARSAbilityInteractVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("USceneComponent");
    SetRootComponent(SceneComponent);

    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereComponent->InitSphereRadius(SphereRadius);
    SphereComponent->SetCollisionProfileName("CharacterMesh");
    SphereComponent->SetGenerateOverlapEvents(true);
    SphereComponent->bHiddenInGame = IsHiddenInGame && !IsSphereForm;
    SphereComponent->SetupAttachment(SceneComponent);

    BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
    BoxComponent->SetBoxExtent(FVector(SphereRadius, SphereRadius, SphereRadius));
    BoxComponent->SetCollisionProfileName("CharacterMesh");
    BoxComponent->SetGenerateOverlapEvents(true);
    BoxComponent->bHiddenInGame = IsHiddenInGame && !IsBoxForm;
    BoxComponent->SetupAttachment(SceneComponent);

    BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARSAbilityInteractVolume::BeginPlay()
{
    Super::BeginPlay();

    // Binding event on overlap begin or end
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARSAbilityInteractVolume::OnOverlapBegin);
    SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ARSAbilityInteractVolume::OnOverlapEnd);
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARSAbilityInteractVolume::OnOverlapBegin);
    BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ARSAbilityInteractVolume::OnOverlapEnd);

    FTimerHandle TempTimerHandle;
    GetWorldTimerManager().SetTimer(TempTimerHandle, [&]()
        {
            if (IsSphereForm)
                SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            else
                BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        }, 0.1,
        false);
}

#if UE_EDITOR

void ARSAbilityInteractVolume::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (!PropertyChangedEvent.Property)
    {
        LOG_RS(ELogRSVerb::Warning, FString::Printf(TEXT("Ability Volume actor: %s have property is nullptr"), *GetName()));
        return;
    }

    if (PropertyChangedEvent.Property->GetName() == TEXT("SphereRadius"))
    {
        SphereComponent->SetSphereRadius(SphereRadius);
        BoxComponent->SetBoxExtent(FVector(SphereRadius, SphereRadius, SphereRadius));
    }
    if (PropertyChangedEvent.Property->GetName() == TEXT("IsSphereForm"))
    {
        BoxComponent->SetGenerateOverlapEvents(false);
        SphereComponent->SetGenerateOverlapEvents(true);
        IsBoxForm = false;
        SphereComponent->bHiddenInGame = IsHiddenInGame && !IsSphereForm;
        BoxComponent->bHiddenInGame = IsHiddenInGame && !IsBoxForm;
    }
    if (PropertyChangedEvent.Property->GetName() == TEXT("IsBoxForm"))
    {
        BoxComponent->SetGenerateOverlapEvents(true);
        SphereComponent->SetGenerateOverlapEvents(false);
        IsSphereForm = false;
        SphereComponent->bHiddenInGame = IsHiddenInGame && !IsSphereForm;
        BoxComponent->bHiddenInGame = IsHiddenInGame && !IsBoxForm;
    }
}

#endif

void ARSAbilityInteractVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Call change on overlap begin with params, that set in component
    SetStateChangedValue(OtherActor, AddValueToState);
}

void ARSAbilityInteractVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    // Call change on overlap begin with params, that set in component, with invert (-)
    SetStateChangedValue(OtherActor, -AddValueToState);
}

void ARSAbilityInteractVolume::SetStateChangedValue(const AActor* Actor, const float AddValue) const
{
    if (!Actor) return;

    URSAbilitySystem* AbilitySystem = Actor->FindComponentByClass<URSAbilitySystem>();
    if (AbilitySystem)
    {
        AbilitySystem->SetChangeValue(AbilityStateType, AddValue);
    }
}
