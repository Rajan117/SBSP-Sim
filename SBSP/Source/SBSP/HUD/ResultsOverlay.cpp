// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultsOverlay.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SBSP/Controllers/SBSPPlayerController.h"

void UResultsOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	if (SaveButton)
	{
		SaveButton->OnClicked.AddDynamic(this, &UResultsOverlay::OnSaveButtonClicked);
	}
}

void UResultsOverlay::SetResults(int32 NumTiles, int32 NumRobots, int32 NumLaunches, float TotalTime, float TotalDistanceRobotsTravelled)
{
	TilesText->SetText(FText::FromString(FString::FromInt(NumTiles)));
	RobotsText->SetText(FText::FromString(FString::FromInt(NumRobots)));
	LaunchesText->SetText(FText::FromString(FString::FromInt(NumLaunches)));
	TimeText->SetText(FText::FromString(FString::SanitizeFloat(TotalTime)));
	DistanceText->SetText(FText::FromString(FString::SanitizeFloat(TotalDistanceRobotsTravelled/100.f)));
}

void UResultsOverlay::OnSaveButtonClicked()
{
	if (ASBSPPlayerController* SBSPPlayerController = Cast<ASBSPPlayerController>(GetOwningPlayer()))
	{
		SBSPPlayerController->SaveSimResultsToCSV();
	}
}

