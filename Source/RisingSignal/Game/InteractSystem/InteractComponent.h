// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Library/RSFunctionLibrary.h"
#include "Player/NewTestPlayer/RSBaseCharacter.h"
//#include "Player/NewTestPlayer/RSBaseCharacter.h"
#include "InteractComponent.generated.h"

class URSInventoryComponent;
class UJournalSystem;
class ARSGamePlayerController;
class AInteractItemActor;
class UBoxComponent;
class ARSGamePLayer;
class URSSearchComponent;

/**
 * @class
 **/
UCLASS(Blueprintable, ClassGroup = (Managers), meta = (BlueprintSpawnableComponent),
    HideCategories = ("Variable", "Transform", "Sockets", "Shape", "Navigation", "ComponentTick", "Physics", "Tags", "Cooking", "HLOD",
        "Mobile", "Activation", "Component Replication", "Events", "Asset User Data", "Collision"))
class RISINGSIGNAL_API UInteractComponent : public UActorComponent
{
    GENERATED_BODY()

#pragma region Default

public:
    UInteractComponent();

protected:
    virtual void BeginPlay() override;

#define LOG_INTERACT(LogVerb, Text) Print_Log(LogVerb, Text, __LINE__, __FUNCTION__)

    void Print_Log(ELogRSVerb LogVerb, FString Text, int Line, const char* Function) const;

#if WITH_EDITOR

    /**
     * @protected Called whenever a variable changes
     **/
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

#pragma endregion

#pragma region DataInteract

public:
    UFUNCTION(BlueprintPure, Category = "UInteractComponent | InteractItem")
    AInteractItemActor* GetInteractItem() const { return TargetInteractItem; }

private:
    // @private Size box collision
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Interact", meta = (AllowPrivateAccess = true))
    FVector SizeBoxCollision{50.f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Interact", meta = (AllowPrivateAccess = true))
    float RateTimeChecked{0.25f};

    // @private Owner Actor component
    UPROPERTY()
    ARSBaseCharacter* OwnerPlayer;

    // @private Owner Controller actor
    UPROPERTY()
    ARSGamePlayerController* PlayerController;

    // @private Inventory component
    UPROPERTY()
    URSInventoryComponent* InventoryComp;

    // @private Pointer on UJournalSystem
    UPROPERTY()
    UJournalSystem* JournalSystem;

    // @private Box Collision component
    UPROPERTY()
    UBoxComponent* BoxCollision;

    // @private Target item from the world
    UPROPERTY()
    AInteractItemActor* TargetInteractItem;

    UPROPERTY()
    URSSearchComponent* TargetSearchItem;

    // @private Array interact item
    UPROPERTY()
    TArray<AInteractItemActor*> ArrInteractItem;

    UPROPERTY()
    TArray<URSSearchComponent*> ArrSearchComp;

    // @private Timer to check the distance of the player to the interactive object
    FTimerHandle CheckedInteractItemTimerHandle;

    UFUNCTION()
    void RegisterBeginOverlapInteractItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void RegisterEndOverlapInteractItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    /**
     * @private Add item
     * @param1 AInteractItemActor
     **/
    void AddItem(AInteractItemActor* InteractItem);

    void AddSearchItem(URSSearchComponent* SearchComponent);

    /**
     * @private Remove item
     * @param1 AInteractItemActor
     **/
    void RemoveItem(AInteractItemActor* InteractItem);

    /**
     * @private Checking the distance and changing the target object
     **/
    void CheckDistanceToItem();

    /**
     * @private register and process target interact actor
     **/
    UFUNCTION()
    void RegisterInteractEvent();

    void SendNoteData(const struct FDataInteract* DataInteract) const;
    void SendAudioData(const struct FDataInteract* DataInteract) const;
    void SendPhotoData(const struct FDataInteract* DataInteract) const;

#pragma endregion

#pragma region ANIMATIONS

    UFUNCTION(BlueprintPure, Category = "UInteractComponent | AnimMontages")
    UAnimMontage* GetGroundPickUpAnimMontage() const { return GroundPickUpAnimMontage; }

    UFUNCTION(BlueprintCallable, Category = "UInteractComponent | AnimMontages")
    void SetGroundPickUpAnimMontage(UAnimMontage* NewAnimMontage) { GroundPickUpAnimMontage = NewAnimMontage; }

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations", DisplayName = "Аниммонтаж подъема с земли",
        meta= (AllowPrivateAccess = true, ToolTip = "Укажите аниммонтаж, который должен проигрываться при подъеме предмета с земли"))
    UAnimMontage* GroundPickUpAnimMontage;

    void InitAnimations();

    void StartPickUpAnimation() const;
    void PickUpAnimationEnded() const;

#pragma endregion ANIMATIONS
};
