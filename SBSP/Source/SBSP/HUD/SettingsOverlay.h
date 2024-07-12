// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBSP/Controllers/SBSPPlayerController.h"
#include "SettingsOverlay.generated.h"

class USpinBox;
class UButton;
/**
 * 
 */
UCLASS()
class SBSP_API USettingsOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;
	//Structure
	UPROPERTY(meta = (BindWidget))
	USpinBox* StructureRadiusInput;
	//Harbour
	UPROPERTY(meta = (BindWidget))
	USpinBox* HarbourRadiusInput;
	UPROPERTY(meta = (BindWidget))
	USpinBox* InitStockInput;
	UPROPERTY(meta = (BindWidget))
	USpinBox* NumRobotsInput;
	//Robots
	UPROPERTY(meta = (BindWidget))
	USpinBox* RobotSpeedInput;
	//Rocket
	UPROPERTY(meta = (BindWidget))
	USpinBox* LaunchFreqInput;
	UPROPERTY(meta = (BindWidget))
	USpinBox* TilePayloadInput;

protected:
	UFUNCTION()
	void OnStartButtonPressed();

private:
	UPROPERTY()
	ASBSPPlayerController* SBSPPlayerController;
	UPROPERTY()
	FSimSettings SimSettings;
};
