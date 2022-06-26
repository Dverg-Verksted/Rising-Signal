// It is owned by the company Dverg Verksted.

#include "Game/InteractSystem/InteractItemActor.h"

#include "InteractItemDataAsset.h"
#include "Engine/AssetManager.h"
#include "Library/RSFunctionLibrary.h"

// Sets default values
AInteractItemActor::AInteractItemActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
    PrimaryActorTick.bAllowTickOnDedicatedServer = false;
    PrimaryActorTick.bTickEvenWhenPaused = false;

    this->Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh component"));
}

// Called when the game starts or when spawned
void AInteractItemActor::BeginPlay()
{
    Super::BeginPlay();
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

    LOG_RS(ELogRSVerb::Display, FString::Printf(TEXT("Name changed property: %s"), *PropertyChangedEvent.Property->GetName()));

    if (PropertyChangedEvent.Property->GetName() == TEXT("InteractItem") && this->InteractItem)
    {
        UStaticMesh* L_Mesh = LoadObject<UStaticMesh>(nullptr, *(this->InteractItem->GetMeshItem().ToString()));
        if (!L_Mesh) return;
        this->Mesh->SetStaticMesh(L_Mesh);
    }
}

#endif
