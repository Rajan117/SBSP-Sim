// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultsOverlay.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class SBSP_API UResultsOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TilesText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RobotsText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LaunchesText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DistanceText;

	void SetResults(int32 NumTiles,
		int32 NumRobots,
		int32 NumLaunches,
		float TotalTime,
		float TotalDistanceRobotsTravelled);
	
};
