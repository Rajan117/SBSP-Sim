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
	CSVLines.Add("StructureRadius," + FString::FromInt(SimSettings.StructureRadius));
	CSVLines.Add("InitStock," + FString::FromInt(SimSettings.InitStock));
	CSVLines.Add("TileRadius," + FString::FromInt(SimSettings.TileRadius));
	CSVLines.Add("NumRobots," + FString::FromInt(SimSettings.NumRobots));
	CSVLines.Add("RobotSpeed," + FString::SanitizeFloat(SimSettings.RobotSpeed));
	CSVLines.Add("LaunchFrequency," + FString::SanitizeFloat(SimSettings.LaunchFrequency));
	CSVLines.Add("TilePayload," + FString::FromInt(SimSettings.TilePayload));

	//Sim Results
	CSVLines.Add("TotalTiles," + FString::FromInt(SimResults.TotalTiles));
	CSVLines.Add("TotalRobots," + FString::FromInt(SimResults.TotalRobots));
	CSVLines.Add("TotalLaunches," + FString::FromInt(SimResults.TotalLaunches));
	CSVLines.Add("TotalTime," + FString::Printf(TEXT("%.2f"), SimResults.TotalTime));
	CSVLines.Add("TotalDistanceRobotsTravelled," + FString::Printf(TEXT("%.2f"), SimResults.TotalDistanceRobotsTravelled/100.f));

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


