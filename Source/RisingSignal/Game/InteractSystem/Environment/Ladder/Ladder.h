// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "Game/InteractSystem/Environment/InteractiveActor.h"
#include "Ladder.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API ALadder : public AInteractiveActor
{
	GENERATED_BODY()
    
public:
    
    ALadder();

    virtual void OnConstruction(const FTransform& Transform) override;

    virtual void BeginPlay() override;

    float GetLadderHeight() const;

    bool GetIsOnTop() const;

    UAnimMontage* GetAttachFromTopAnimMontage() const;

    UCurveFloat* GetAttachCurve() const { return AttachFromTopCurve; }

    FVector GetAttachFromTopEndPosition() const;

    FVector GetAttachFromTopStartPosition() const { return AttachFromTopStartPosition; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters", DisplayName="Высота лестницы")
    float LadderHeight = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters", DisplayName="Ширина лестницы")
    float LadderWeight = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters", DisplayName="Интервал ступенек")
    float StepsInterval = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters", DisplayName="Смещение нижней ступеньки",
        meta=(ToolTip="Задает смещение, с которого начинается самая нижняя ступенька."))
    float BottomStepOffset = 25.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters", DisplayName="Анимация залаза на лестницу сверху")
    UAnimMontage* AttachFromTopAnimMontage;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters", DisplayName="Кривая скорости",
        meta=(ToolTip="Задает скорость перемещения персонажа из верхней точки начала анимации в конечную в зависимости от времени анимации"))
    UCurveFloat* AttachFromTopCurve;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters", DisplayName="Верхняя позиция",
        meta=(MakeEditWidget, ToolTip="Позиция в которую переместиться персонаж при взаимодействии с лестницей и с которой начнется анимация залаза сверху. Можно редактировать только X и Y координаты."))
    FVector AttachFromTopStartPosition = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters", DisplayName="Смещение верхней позиции",
        meta=(ToolTip="Значение обозночающее высоту верхней позиции от самого верха лестницы."))
    float OffsetZFromTopStartPosition = 100.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ladder parameters", DisplayName="Смещение конечной позиции",
        meta=(ToolTip="Смещение нижней позиции, в которой игрок окажется после проигрывании анимации залаза сверху"))
    FVector OffsetAttachFromTopEndPosition = FVector(50.0f, 0.0f, -90.0f);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* RightRailMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* LeftRailMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UInstancedStaticMeshComponent* StepsMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UBoxComponent* TopInteractionVolume;
    
    virtual void OnInteractionVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    virtual void OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

    UBoxComponent* GetMeshInteractiveBox();

private:
    bool bIsOnTop = false;
};
