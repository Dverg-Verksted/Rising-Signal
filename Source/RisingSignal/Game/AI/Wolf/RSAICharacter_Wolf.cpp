// It is owned by the company Dverg Verksted.


#include "Game/AI/Wolf/RSAICharacter_Wolf.h"

#include "Components/TimelineComponent.h"
#include "Game/AI/RSAIController.h"
#include "Library/RSFunctionLibrary.h"

ARSAICharacter_Wolf::ARSAICharacter_Wolf()
{
}

void ARSAICharacter_Wolf::BeginPlay()
{
    Super::BeginPlay();

    check(AIController);
    if (SwitchColorCurve)
    {
        FOnTimelineFloat TimelineProgress;
        TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
        SwitchColorTimeline.AddInterpFloat(SwitchColorCurve, TimelineProgress);
        SwitchColorTimeline.SetLooping(false);
    }
}

void ARSAICharacter_Wolf::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    SwitchColorTimeline.TickTimeline(DeltaSeconds);
}

void ARSAICharacter_Wolf::TimelineProgress(float Value)
{
    GetMesh()->SetScalarParameterValueOnMaterials("SwitchValue", Value);
}


void ARSAICharacter_Wolf::EnemyNoticed(bool IsNoticed)
{
    Super::EnemyNoticed(IsNoticed);

    if (IsNoticed || CurrentAIState == Attack || CurrentAIState == Threaten)
    {
        SwitchWolfColorToBlack(true);
    }
    else
    {
        SwitchWolfColorToBlack(false);
    }
}


void ARSAICharacter_Wolf::SwitchWolfColorToBlack(bool bSwitch)
{
    if (bSwitch)
    {
        SwitchColorTimeline.Play();
    }
    else
    {
        SwitchColorTimeline.Reverse();
    }
}
