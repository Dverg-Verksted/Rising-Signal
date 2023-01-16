// It is owned by the company Dverg Verksted.

#pragma once

#include "CoreMinimal.h"
#include "CableComponent.h"
#include "Components/BoxComponent.h"
#include "Game/InteractSystem/Environment/InteractiveActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Rope.generated.h"

/**
 * 
 */
UCLASS()
class RISINGSIGNAL_API ARope : public AInteractiveActor
{
	GENERATED_BODY()
    
public:

    ARope();

    virtual void OnConstruction(const FTransform& Transform) override;

    void AddSwingForce(FVector& Force, bool bForce);

    FORCEINLINE UStaticMeshComponent* GetCableEndMeshComponent() const { return CableEndMeshComponent; }

    FORCEINLINE float GetLaunchY() const { return LaunchYMultiplier; }
    FORCEINLINE float GetLaunchZ() const { return LaunchZValue; }
    
protected:

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rope|Parameters", DisplayName="Сила раскачки",
    meta=(ToolTip="Сила которая будет применяться к веревке, при нажатии кнопок движения"))
    float SwingMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rope|Parameters", DisplayName="Длина веревки",
    meta=(ToolTip="Длина основной веревки, которая начинается с самого верха и протянута до CableEndMeshComponent"))
    float CableLength;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rope|Parameters", DisplayName="Сторона расскачки",
    meta=(ToolTip="При значении True раскачивание верёвки будет происходить слева направо при захвате, при установке False справа налево."))
    bool InertionLR;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rope|Parameters", DisplayName="Ограничение на расскачивание",
    meta=(ToolTip="Чем ниже установленное значение тем быстрее персонаж будет расскачивать верёвку. 1000 - отключение раскачивания;"))
    float SwingLimit;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rope|Parameters", DisplayName="Множитель прыжка Y",
    meta=(ToolTip="Множитель, который умножается на силу расскачки веревки и получаемое значение, применяется к персонажу. Чем больше множитель, тем дальше персонаж будет улетать."))
    float LaunchYMultiplier = 1.2f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rope|Parameters", DisplayName="Сила прыжка Z",
    meta=(ToolTip="Сила по оси Z, которое будет применяться к персонажу при прыжке. Чем больше, тем выше персонаж будет взлетать."))
    float LaunchZValue = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    UCableComponent* CableComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* CableEndMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    UCableComponent* EndCable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    UPhysicsConstraintComponent* PhysicsConstraintComponent;

    UBoxComponent* GetInteractionBox() {return StaticCast<UBoxComponent*>(InteractionVolume); }

    virtual void OnInteractionVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    virtual void OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
