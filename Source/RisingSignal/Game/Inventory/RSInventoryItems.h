// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RSInventoryItems.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API URSInventoryItems : public UObject
{
	GENERATED_BODY()

private:
	int32 IndexItem=-1;
	int32 Count=0;
	TShaderRef<UTexture2D> ImageItem;

public:

	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Инвентарь")
	bool bICanCraft=false;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Инвентарь")
	bool bICanUse=false;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Инвентарь")
	int32 MaxICount=50;
};
