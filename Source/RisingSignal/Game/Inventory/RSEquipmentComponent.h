// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "RSInventoryComponent.h"
#include "Components/ActorComponent.h"
#include "RSEquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentSlotChangedSignature, FInventoryItem, Item);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RISINGSIGNAL_API URSEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URSEquipmentComponent();

    UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite)
    FOnEquipmentSlotChangedSignature OnEquipmentSlotChanged;

    void EquipItemInSlot(const FInventoryItem& Item, int32 Index);
    void UnEquipItemFromSlot(const FInventoryItem& Item);

    void TakeInHands(int32 Index);

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Equipment")
    TMap<int32, FInventoryItem> EquipmentSlots;

private:

    void UpdateEquipmentSlot(int32 Index, const FInventoryItem& Item);
    
    int32 CurrentItemInHand;

		
};
