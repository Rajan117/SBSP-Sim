// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultsOverlay.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SBSP_API UResultsOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
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
	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	void SetResults(int32 NumTiles,
		int32 NumRobots,
		int32 NumLaunches,
		float TotalTime,
		float TotalDistanceRobotsTravelled);

protected:
	UFUNCTION()
	void OnSaveButtonClicked();
	
};
