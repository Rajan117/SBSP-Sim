// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsOverlay.h"

#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SBSP/Controllers/SBSPPlayerController.h"

void USettingsOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	SBSPPlayerController = Cast<ASBSPPlayerController>(GetOwningPlayer());

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &USettingsOverlay::OnStartButtonPressed);
	}
}

void USettingsOverlay::OnStartButtonPressed()
{
	SimSettings.StructureRadius = StructureRadiusInput->GetValue();
	SimSettings.TileRadius = HarbourRadiusInput->GetValue();
	SimSettings.InitStock = InitStockInput->GetValue();
	SimSettings.NumRobots = NumRobotsInput->GetValue();
	SimSettings.RobotSpeed = RobotSpeedInput->GetValue();
	SimSettings.LaunchFrequency = LaunchFreqInput->GetValue();
	SimSettings.TilePayload = TilePayloadInput->GetValue();
	SBSPPlayerController->StartSimulation(SimSettings);
}
