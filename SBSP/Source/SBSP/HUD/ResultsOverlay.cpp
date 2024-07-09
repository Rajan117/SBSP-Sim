// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultsOverlay.h"

#include "Components/TextBlock.h"

void UResultsOverlay::SetResults(int32 NumTiles, int32 NumRobots, int32 NumLaunches, float TotalTime)
{
	TilesText->SetText(FText::FromString(FString::FromInt(NumTiles)));
	RobotsText->SetText(FText::FromString(FString::FromInt(NumRobots)));
	LaunchesText->SetText(FText::FromString(FString::FromInt(NumLaunches)));
	TimeText->SetText(FText::FromString(FString::SanitizeFloat(TotalTime)));
}
