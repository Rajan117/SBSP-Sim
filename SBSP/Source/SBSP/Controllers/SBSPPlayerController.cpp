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

	if (ASBSPHUD* HUD = Cast<ASBSPHUD>(GetHUD()))
	{
		SBSPHUD = HUD;
		SBSPHUD->AddSettingsOverlay();
	}
}

void ASBSPPlayerController::StartSimulation(FSimSettings InSimSettings)
{
	SimSettings = InSimSettings;
	SBSPHUD->RemoveSettingsOverlays();
	SBSPHUD->AddSimOverlay();
	SpawnStructure(InSimSettings);
}

void ASBSPPlayerController::SaveSimResultsToCSV()
{
	FString SaveDirectory = FPaths::ProjectDir();
	FString FileName = "SimulationResults_" + FDateTime::Now().ToString() + ".csv";
	FString FilePath = SaveDirectory + FileName;

	TArray<FString> CSVLines;
	
	//Sim Settings
	CSVLines.Add(TEXT("Setting,Value"));

	CSVLines.Add(TEXT("StructureRadius," + SimSettings.StructureRadius));
	CSVLines.Add(TEXT("InitStock," + SimSettings.InitStock));
	CSVLines.Add(TEXT("TileRadius," + SimSettings.TileRadius));
	CSVLines.Add(TEXT("NumRobots," + SimSettings.NumRobots));
	CSVLines.Add(TEXT("RobotSpeed," + FString::SanitizeFloat(SimSettings.RobotSpeed)));
	CSVLines.Add(TEXT("LaunchFrequency," + FString::SanitizeFloat(SimSettings.LaunchFrequency)));
	CSVLines.Add(TEXT("TilePayload," + SimSettings.TilePayload));

	//SimResults
	CSVLines.Add(TEXT("Result,Value"));

	CSVLines.Add(TEXT("TotalTiles," + SimResults.TotalTiles));
	CSVLines.Add(TEXT("TotalRobots," + SimResults.TotalRobots));
	CSVLines.Add(TEXT("TotalLaunches," + SimResults.TotalLaunches));
	CSVLines.Add(TEXT("TotalTime," + FString::SanitizeFloat(SimResults.TotalTime)));
	CSVLines.Add(TEXT("TotalDistanceRobotsTravelled," + FString::SanitizeFloat(SimResults.TotalDistanceRobotsTravelled)));

	if (bool bIsSaved = FFileHelper::SaveStringArrayToFile(CSVLines, *FilePath))
	{
		UKismetSystemLibrary::PrintString(this, "Saved results to: " + FilePath);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, "Failed to save results to: " + FilePath);
	}
}

void ASBSPPlayerController::SpawnStructure(FSimSettings InSimSettings)
{
	if (SpaceStructureClass)
	{
		SpaceStructure = Cast<ASpaceStructure>(GetWorld()->SpawnActor(SpaceStructureClass));
		if (SpaceStructure)
		{
			SpaceStructure->OnStructureCompletedDelegate.AddDynamic(this, &ASBSPPlayerController::OnSpaceStructureCompleted);
			SpaceStructure->SpawnInit(this, InSimSettings);
		}
	}
}

void ASBSPPlayerController::OnSpaceStructureCompleted(int32 NumTiles, int32 NumRobots, int32 NumLaunches,
	float TotalTime, float TotalDistanceRobotsTravelled)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	if (SBSPHUD)
	{
		SBSPHUD->RemoveSimOverlay();
		SBSPHUD->AddResultsOverlay(NumTiles, NumRobots, NumLaunches, TotalTime, TotalDistanceRobotsTravelled);
	}
	SimResults.TotalTiles = NumTiles;
	SimResults.TotalRobots = NumRobots;
	SimResults.TotalLaunches = NumLaunches;
	SimResults.TotalTime = TotalTime;
	SimResults.TotalDistanceRobotsTravelled = TotalDistanceRobotsTravelled;
}


