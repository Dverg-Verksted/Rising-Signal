// It is owned by the company Dverg Verksted.

#include "Player/RSGamePLayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARSGamePLayer::ARSGamePLayer()
{
    this->SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("Spring arm component"));
    this->SpringArm->SetupAttachment(GetRootComponent());
    this->SpringArm->bUsePawnControlRotation = true;

    this->Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera component"));
    this->Camera->SetupAttachment(this->SpringArm);
}

void ARSGamePLayer::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ARSGamePLayer::BeginPlay()
{
    Super::BeginPlay();
}
