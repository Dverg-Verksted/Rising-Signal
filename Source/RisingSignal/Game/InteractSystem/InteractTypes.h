#pragma once

#include "Engine/DataTable.h"
#include "InteractTypes.generated.h"

// base interact types

USTRUCT()
struct FNeededItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, DisplayName = "Предмет")
    FDataTableRowHandle ItemRowHandle;

    UPROPERTY(EditAnywhere, DisplayName = "Количество")
    float ItemCount = 1;
};


// bonfire types

UENUM()
enum class EFireSize
{
    None UMETA(DisplayName = "Отсутствует"),
    Low UMETA(DisplayName = "Слабый"),
    Medium UMETA(DisplayName = "Средний"),
    Big UMETA(DisplayName = "Сильный")
};

UENUM()
enum class EBonfireType : uint8
{
    None,
    SmallFire UMETA(DisplayName = "Малый костер"),
    BigFire UMETA(DisplayName = "Большой костер")
};
