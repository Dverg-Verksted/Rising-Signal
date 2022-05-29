// It is owned by the company Dverg Verksted.

#include "RSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RSGamePlayerController.h"

void ARSGameMode::BeginPlay()
{
	Super::BeginPlay();

	this->PC = Cast<ARSGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	check(this->PC);

	this->GameHUD = Cast<AGameHUD>(this->PC->GetHUD());
	check(this->GameHUD);
}
