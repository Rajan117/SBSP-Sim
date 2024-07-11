// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SBSPHUD.generated.h"

class USettingsOverlay;
class UResultsOverlay;
class USimOverlay;

UCLASS()
class SBSP_API ASBSPHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASBSPHUD();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	void AddSimOverlay();
	void RemoveSimOverlay();
	void AddResultsOverlay(int32 NumTiles, int32 NumRobots, int32 NumLaunches, float TotalTime);
	void RemoveResultsOverlay();
	void AddSettingsOverlay();
	void RemoveSettingsOverlays();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USimOverlay> SimOverlayClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UResultsOverlay> ResultsOverlayClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USettingsOverlay> SettingsOverlayClass;

private:
	UPROPERTY()
	USimOverlay* SimOverlayRef;
	UPROPERTY()
	UResultsOverlay* ResultsOverlayRef;
	UPROPERTY()
	USettingsOverlay* SettingsOverlayRef;
};
