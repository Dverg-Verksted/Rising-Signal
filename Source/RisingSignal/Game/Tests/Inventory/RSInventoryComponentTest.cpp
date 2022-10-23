// It is owned by the company Dverg Verksted.


#include "Game/Tests/Inventory/RSInventoryComponentTest.h"
#include "CoreMinimal.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AutomationTest.h"
#include "Player/RSGamePLayer.h"
#include "Tests/AutomationCommon.h"



/*IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindItem, "RS.InventoryComponent.FindItem",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);*/

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestCraft, "RS.InventoryComponent.TestCraft",
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


/*bool FFindItem::RunTest(const FString& Parameters)
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

    FTimerHandle TimerHandle;
    World->GetTimerManager().SetTimer(TimerHandle, [=]()
        {
            TArray<FInventoryItem> NeedItems;
            FInventoryItem NeedItem;
            NeedItem.InteractRowName = FName("Test");
            NeedItems.Add(NeedItem);
            //InventoryComponent->FindItemsToUse(NeedItems);
        }, 10.0f, false);

    return true;
}*/

bool FTestCraft::RunTest(const FString& Parameters)
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

    for(int i = 0; i < 2; i++)
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

    for(int i = 0; i < 2; i++)
    {
        FDataTableRowHandle DataTableRowHandle;
        DataTableRowHandle.DataTable = DataTable;
        DataTableRowHandle.RowName = FName("MetalScrap");

        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [=]()
        {
            InventoryComponent->AddDataItem(DataTableRowHandle, "MetalScrap", 5);
        }, 1.0f, false);
    }

    for(int i = 0; i < 2; i++)
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

    for(int i = 0; i < 2; i++)
    {
        FDataTableRowHandle DataTableRowHandle;
        DataTableRowHandle.DataTable = DataTable;
        DataTableRowHandle.RowName = FName("Berries");

        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [=]()
        {
            InventoryComponent->AddDataItem(DataTableRowHandle, "Berries", 5);
        }, 1.0f, false);
    }

    for(int i = 0; i < 2; i++)
    {
        FDataTableRowHandle DataTableRowHandle;
        DataTableRowHandle.DataTable = DataTable;
        DataTableRowHandle.RowName = FName("WoodStick");

        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [=]()
        {
            InventoryComponent->AddDataItem(DataTableRowHandle, "WoodStick", 5);
        }, 1.0f, false);
    }

    for(int i = 0; i < 2; i++)
    {
        FDataTableRowHandle DataTableRowHandle;
        DataTableRowHandle.DataTable = DataTable;
        DataTableRowHandle.RowName = FName("Gas");

        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [=]()
        {
            InventoryComponent->AddDataItem(DataTableRowHandle, "Gas", 5);
        }, 1.0f, false);
    }

    return true;
}









