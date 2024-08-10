// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SBSPPlayerController.generated.h"

class ASpaceStructure;
class ASBSPHUD;

USTRUCT()
struct FSimSettings
{
	GENERATED_BODY()
	//Space Structure Properties
	int32 StructureRadius = 2;
	//Harbour Properties
	int32 InitStock = 50;
	int32 TileRadius = 4;
	int32 NumRobots = 5;
	//RobotProperties
	float RobotSpeed = 1.f;
	//RocketProperties
	float LaunchFrequency = 10.f;
	int32 TilePayload = 10;
};

USTRUCT()
struct FSimResults
{
	GENERATED_BODY()
	int32 TotalTiles = 0;
	int32 TotalRobots = 0;
	int32 TotalLaunches = 0;
	float TotalTime = 0.f;
	float TotalDistanceRobotsTravelled = 0.f;
};

UCLASS()
class SBSP_API ASBSPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASBSPPlayerController();
	virtual void BeginPlay() override;
	void StartSimulation(FSimSettings InSimSettings);
	void SaveSimResultsToCSV();

protected:
	void SpawnStructure(FSimSettings InSimSettings);
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpaceStructure> SpaceStructureClass;
	UFUNCTION()
	void OnSpaceStructureCompleted(int32 NumTiles,
		int32 NumRobots,
		int32 NumLaunches,
		float TotalTime,
		float TotalDistanceRobotsTravelled);
	


private:
	UPROPERTY()
	ASBSPHUD* SBSPHUD;
	UPROPERTY()
	ASpaceStructure* SpaceStructure;
	FSimSettings SimSettings;
	FSimResults SimResults;
	
public:
	FORCEINLINE ASpaceStructure* GetSpaceStructure() const { return  SpaceStructure; }

};
