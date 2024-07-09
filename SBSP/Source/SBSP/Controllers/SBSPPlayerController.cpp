// Fill out your copyright notice in the Description page of Project Settings.


#include "SBSPPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "SBSP/HUD/SBSPHUD.h"
#include "SBSP/SpaceStructures/SpaceStructure.h"

ASBSPPlayerController::ASBSPPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASBSPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameAndUI InputModeGameAndUI = FInputModeGameAndUI();
	SetInputMode(InputModeGameAndUI);
	SetShowMouseCursor(true);

	SpawnStructure();
	if (ASBSPHUD* HUD = Cast<ASBSPHUD>(GetHUD()))
	{
		SBSPHUD = HUD;
		SBSPHUD->AddSimOverlay();
	}
}

void ASBSPPlayerController::SpawnStructure()
{
	if (SpaceStructureClass)
	{
		SpaceStructure = Cast<ASpaceStructure>(GetWorld()->SpawnActor(SpaceStructureClass));
		if (SpaceStructure)
		{
			SpaceStructure->OnStructureCompletedDelegate.AddDynamic(this, &ASBSPPlayerController::OnSpaceStructureCompleted);
		}
	}
}

void ASBSPPlayerController::OnSpaceStructureCompleted(int32 NumTiles, int32 NumRobots, int32 NumLaunches,
	float TotalTime)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	if (SBSPHUD)
	{
		SBSPHUD->RemoveSimOverlay();
		SBSPHUD->AddResultsOverlay(NumTiles, NumRobots, NumLaunches, TotalTime);
	}
}


