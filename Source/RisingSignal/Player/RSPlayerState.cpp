// It is owned by the company Dverg Verksted.


#include "Player/RSPlayerState.h"

#include "RSGamePlayerController.h"
#include "Game/Inventory/RSCraftComponent.h"
#include "Game/Inventory/RSEquipmentComponent.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "Game/SaveLoad/RSSaveGame.h"
#include "Library/RSFunctionLibrary.h"
#include "NewTestPlayer/RSBaseCharacter.h"

void ARSPlayerState::SavePlayerState_Implementation(URSSaveGame* SaveObject)
{

    if (SaveObject)
    {
        // Gather all relevant data for player
        FPlayerSaveData SaveData;

        // Stored as FString for simplicity (original Steam ID is uint64)
        SaveData.PlayerID = GetUniqueId().ToString();

        // May not be alive while we save
        ARSBaseCharacter* RSChar = GetPawn<ARSBaseCharacter>();
        if (RSChar)
        {
            SaveData.Location = RSChar->GetActorLocation();
            SaveData.Rotation = RSChar->GetActorRotation();
            SaveData.bResumeAtTransform = true;

            URSAbilitySystem* AbilitySystem = RSChar->GetAbilitySystem();
            if (AbilitySystem)
            {
                for (const auto State : AbilitySystem->States)
                {

                    SaveData.AbilityParams.Add(State);
                }
            }
            else
            {
                LOG_RS(ELogRSVerb::Error, "Not found AbilitySystem comp");
            }

            URSInventoryComponent* Inventory = RSChar->GetInventoryComponent();
            if (Inventory)
            {
                SaveData.InventoryItems = Inventory->GetItems();

            }
            else
            {
                LOG_RS(ELogRSVerb::Error, "Not found Inventory comp");
            }

            URSCraftComponent* Craft = RSChar->FindComponentByClass<URSCraftComponent>();
            if (Craft)
            {
                SaveData.CraftingItems = Craft->GetItems();

            }
            else
            {
                LOG_RS(ELogRSVerb::Error, "Not found Craft comp");
            }

            URSEquipmentComponent* Equipment = RSChar->FindComponentByClass<URSEquipmentComponent>();
            if (Equipment)
            {
                SaveData.EquipmentSlots = Equipment->GetItems();
                SaveData.CurrentItemInHand = Equipment->GetEquippedItem();

            }
            else
            {
                LOG_RS(ELogRSVerb::Error, "Not found Equipment comp");
            }
        }
        else
        {
            LOG_RS(ELogRSVerb::Error, "No Player found!");
        }

        SaveObject->SavedPlayer = SaveData;

        LOG_RS(ELogRSVerb::Warning, "Player Saved!");
    }
}

void ARSPlayerState::LoadPlayerState_Implementation(URSSaveGame* SaveObject)
{
    if (SaveObject)
    {
        FPlayerSaveData* FoundData = SaveObject->GetPlayerData();
        if (FoundData)
        {
            ARSBaseCharacter* RSChar = GetPawn<ARSBaseCharacter>();

            if (RSChar)
            {
                URSAbilitySystem* AbilitySystem = RSChar->GetAbilitySystem();
                if (AbilitySystem)
                {
                    AbilitySystem->States = FoundData->AbilityParams;
                }
                else
                {
                    LOG_RS(ELogRSVerb::Error, "Not found AbilitySystem comp");
                }

                URSInventoryComponent* Inventory = RSChar->GetInventoryComponent();
                if (Inventory)
                {

                    Inventory->LoadItems(FoundData->InventoryItems);
                }
                else
                {
                    LOG_RS(ELogRSVerb::Error, "Not found Inventory comp");
                }

                URSCraftComponent* Craft = RSChar->FindComponentByClass<URSCraftComponent>();
                if (Craft)
                {

                    Craft->LoadItems(FoundData->CraftingItems);
                }
                else
                {
                    LOG_RS(ELogRSVerb::Error, "Not found Craft comp");
                }

                URSEquipmentComponent* Equipment = RSChar->FindComponentByClass<URSEquipmentComponent>();
                if (Equipment)
                {

                    Equipment->LoadItems(FoundData->EquipmentSlots, FoundData->CurrentItemInHand);
                }
                else
                {
                    LOG_RS(ELogRSVerb::Error, "Not found Equipment comp");
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Could not find SaveGame data for player id '%i'."), GetPlayerId());
        }
    }
}
