// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceStructure.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnStructureCompleted, int32, NumTiles, int32, NumRobots, int32, NumLaunches, float, TotalTime);

class ASBSPPlayerController;
class UResultsOverlay;
class AHexGrid;

UCLASS()
class SBSP_API ASpaceStructure : public AActor
{
	GENERATED_BODY()

public:
	ASpaceStructure();
	virtual void Tick(float DeltaTime) override;

	float GetMeshRadius() const;
	void SpawnInit(ASBSPPlayerController* OwningSBSPPlayerController);
	FOnStructureCompleted OnStructureCompletedDelegate;
	
protected:
	virtual void BeginPlay() override;

	void GenerateHarbourLocations();
	UFUNCTION()
	void OnHarbourCompleted(int32 NumTiles, int32 NumRobots, int32 NumLaunches);
	void HandleCompletion();
	
	//Harbours and Tiles
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* HexTileMesh;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHexGrid> HarbourClass;
	UPROPERTY(EditDefaultsOnly)
	int32 HarbourTileRadius = 6;
	UPROPERTY(EditDefaultsOnly)
	int32 RadiusInHarbours = 2;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UResultsOverlay> ResultsWidgetClass;

private:
	UPROPERTY()
	TArray<AHexGrid*> Harbours;
	int32 NumCompletedHarbours;
	float LongRadius;
	void SpawnHarbour(const FVector& SpawnLocation);
	UPROPERTY()
	ASBSPPlayerController* SBSPPlayerController;
	
	//Stats
	int32 TotalTiles = 0;
	int32 TotalRobots = 0;
	int32 TotalLaunches = 0;
	float TotalTime = 0.f;
	float StartTime = 0.f;
};
