// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Game/Inventory/InventoryTypes.h"
#include "Game/AbilitySystem/BaseComponents/RSAbilitySystem.h"
#include "RSSaveGame.generated.h"


USTRUCT()
struct FActorSaveData
{
    GENERATED_BODY()

public:
    /* Identifier for which Actor this belongs to */
    UPROPERTY()
    FName ActorName;

    /* For movable Actors, keep location, rotation, scale. */
    UPROPERTY()
    FTransform Transform;

    /* Contains all 'SaveGame' marked variables of the Actor */
    UPROPERTY()
    TArray<uint8> ByteData;
};

USTRUCT()
struct FPlayerSaveData
{
    GENERATED_BODY()

public:
    /* Player Id defined by the online sub system (such as Steam) converted to FString for simplicity  */
    UPROPERTY()
    FString PlayerID = "None";

    /* Location if player was alive during save */
    UPROPERTY()
    FVector Location;

    /* Orientation if player was alive during save */
    UPROPERTY()
    FRotator Rotation;

    /* We don't always want to restore location, and may just resume player at specific respawn point in world. */
    UPROPERTY()
    bool bResumeAtTransform = false;

    UPROPERTY()
    TArray<FStateParams> AbilityParams;

    UPROPERTY()
    TArray<FInventoryItem> InventoryItems;

    UPROPERTY()
    TArray<FInventoryItem> CraftingItems;

    UPROPERTY()
    TMap<int32, FInventoryItem> EquipmentSlots;

    UPROPERTY()
    int32 CurrentItemInHand = 0;
};

/**
 * Class for saving and loading
 */

UCLASS()
class RISINGSIGNAL_API URSSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FName SavedLevelName = NAME_None;

    UPROPERTY()
    FPlayerSaveData SavedPlayer{};

    /* Actors stored from a level (currently does not support a specific level and just assumes the demo map) */
    UPROPERTY()
    TArray<FActorSaveData> SavedActors;

    UPROPERTY()
    TArray<FActorSaveData> SavedSpawnedActors;

    FPlayerSaveData* GetPlayerData();

    void ClearData();

};
