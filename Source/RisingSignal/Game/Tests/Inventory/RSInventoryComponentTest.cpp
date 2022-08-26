// It is owned by the company Dverg Verksted.


#include "Game/Tests/Inventory/RSInventoryComponentTest.h"
#include "CoreMinimal.h"
#include "Game/Inventory/RSInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AutomationTest.h"
#include "Player/RSGamePLayer.h"
#include "Tests/AutomationCommon.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddStickOneItem, "RS.InventoryComponent.AddStickItemStackable",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddFewStickItemsCount5, "RS.InventoryComponent.AddFewStickItemsStackable",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddStoneItem, "RS.InventoryComponent.AddStoneItemStackable",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddFewStoneItemsCount5, "RS.InventoryComponent.AddFewStoneItemsStackable",
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

bool FAddStickOneItem::RunTest(const FString& Parameters)
{
    AutomationOpenMap("/Game/RisingSignal/Maps/Map_TEST_InventoryComponent");
    
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
    DataTableRowHandle.RowName = FName("Stick");

    FTimerHandle TimerHandle;
    World->GetTimerManager().SetTimer(TimerHandle, [=]()
    {
        InventoryComponent->AddDataItem(DataTableRowHandle, 1);
    }, 1.0f, false);


    return true;
}

bool FAddFewStickItemsCount5::RunTest(const FString& Parameters)
{
    AutomationOpenMap("/Game/RisingSignal/Maps/Map_TEST_InventoryComponent");
    
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
        DataTableRowHandle.RowName = FName("Stick");

        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [=]()
        {
            InventoryComponent->AddDataItem(DataTableRowHandle, 5);
        }, 1.0f, false);
    }


    return true;
}

bool FAddStoneItem::RunTest(const FString& Parameters)
{
    AutomationOpenMap("/Game/RisingSignal/Maps/Map_TEST_InventoryComponent");
    
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
    DataTableRowHandle.RowName = FName("Stone");

    FTimerHandle TimerHandle;
    World->GetTimerManager().SetTimer(TimerHandle, [=]()
    {
        InventoryComponent->AddDataItem(DataTableRowHandle, 1);
    }, 1.0f, false);


    return true;
}

bool FAddFewStoneItemsCount5::RunTest(const FString& Parameters)
{
    AutomationOpenMap("/Game/RisingSignal/Maps/Map_TEST_InventoryComponent");
    
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
        DataTableRowHandle.RowName = FName("Stone");

        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [=]()
        {
            InventoryComponent->AddDataItem(DataTableRowHandle, 5);
        }, 1.0f, false);
    }

    return true;
}



