// It is owned by the company Dverg Verksted.


#include "Game/Tests/Inventory/RSInventoryComponentTest.h"
#include "CoreMinimal.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AutomationTest.h"
#include "Player/RSGamePLayer.h"
#include "Tests/AutomationCommon.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddFewPoultryEggsItemsCount5Stackable, "RS.InventoryComponent.AddFewPoultryEggsItemsStackable",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBottleFewItemsNonStackable, "RS.InventoryComponent.AddFewBottleItemsNonStackable",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBottleAndUseItem, "RS.InventoryComponent.AddBottleAndUseItem",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace
{
    UWorld* GetTestGameWorld()
    {
        const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
        for ( const FWorldContext& Context : WorldContexts )
        {
            if ( ( ( Context.WorldType == EWorldType::PIE ) || ( Context.WorldType == EWorldType::Game ) ) && ( Context.World() != NULL ) )
            {
                return Context.World();
            }
        }
        return nullptr;
    }
}


bool FAddFewPoultryEggsItemsCount5Stackable::RunTest(const FString& Parameters)
{
    AutomationOpenMap("/Game/RisingSignal/Maps/DevMaps/Map_TEST_InventoryComponent");
    
    UWorld* World = GetTestGameWorld();
    if(!TestNotNull("World exists", World)) return false;

    ARSGamePLayer* Character = StaticCast<ARSGamePLayer*>(UGameplayStatics::GetPlayerCharacter(World, 0));
    if(!TestNotNull("Character exists", Character)) return false;

    URSInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
    if(!TestNotNull("Inventory component exists", InventoryComponent)) return false;

    UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/RisingSignal/Core/Inventory/DT_InventoryItemRules.DT_InventoryItemRules"));
    if(!TestNotNull("DataTable exists", DataTable)) return false;

    for(int i = 0; i < 5; i++)
    {
        FDataTableRowHandle DataTableRowHandle;
        DataTableRowHandle.DataTable = DataTable;
        DataTableRowHandle.RowName = FName("PoultryEggs");

        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [=]()
        {
            InventoryComponent->AddDataItem(DataTableRowHandle, "PoultryEggs", 5);
        }, 1.0f, false);
    }


    return true;
}

bool FAddBottleFewItemsNonStackable::RunTest(const FString& Parameters)
{
    AutomationOpenMap("/Game/RisingSignal/Maps/DevMaps/Map_TEST_InventoryComponent");
    
    UWorld* World = GetTestGameWorld();
    if(!TestNotNull("World exists", World)) return false;

    ARSGamePLayer* Character = StaticCast<ARSGamePLayer*>(UGameplayStatics::GetPlayerCharacter(World, 0));
    if(!TestNotNull("Character exists", Character)) return false;

    URSInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
    if(!TestNotNull("Inventory component exists", InventoryComponent)) return false;

    UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/RisingSignal/Core/Inventory/DT_InventoryItemRules.DT_InventoryItemRules"));
    if(!TestNotNull("DataTable exists", DataTable)) return false;

    for(int i = 0; i < 5; i++)
    {
        FDataTableRowHandle DataTableRowHandle;
        DataTableRowHandle.DataTable = DataTable;
        DataTableRowHandle.RowName = FName("FoodBottleOfWater");

        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [=]()
        {
            InventoryComponent->AddDataItem(DataTableRowHandle, "FoodBottleOfWater", 5);
        }, 1.0f, false);
    }

    return true;
}

bool FAddBottleAndUseItem::RunTest(const FString& Parameters)
{
    AutomationOpenMap("/Game/RisingSignal/Maps/DevMaps/Map_TEST_InventoryComponent");
    
    UWorld* World = GetTestGameWorld();
    if(!TestNotNull("World exists", World)) return false;

    ARSGamePLayer* Character = StaticCast<ARSGamePLayer*>(UGameplayStatics::GetPlayerCharacter(World, 0));
    if(!TestNotNull("Character exists", Character)) return false;

    URSInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
    if(!TestNotNull("Inventory component exists", InventoryComponent)) return false;

    UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/RisingSignal/Core/Inventory/DT_InventoryItemRules.DT_InventoryItemRules"));
    if(!TestNotNull("DataTable exists", DataTable)) return false;
    
    FDataTableRowHandle DataTableRowHandle;
    DataTableRowHandle.DataTable = DataTable;
    DataTableRowHandle.RowName = FName("FoodBottleOfWater");

    FTimerHandle TimerHandle;
    World->GetTimerManager().SetTimer(TimerHandle, [=]()
    {
        InventoryComponent->AddDataItem(DataTableRowHandle, "FoodBottleOfWater", 1);
        TArray<FInventoryItem> Items;
        Items = InventoryComponent->GetItems();
        InventoryComponent->UseItem(Items[0]);
    }, 1.0f, false);


    return true;
}






