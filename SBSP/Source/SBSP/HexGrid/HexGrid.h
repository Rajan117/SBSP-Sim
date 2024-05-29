// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ChildActorComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexGrid.generated.h"

class AHexTile;
class AConstructionRobot;

UCLASS()
class SBSP_API AHexGrid : public AActor
{
	GENERATED_BODY()

public:
	AHexGrid();

protected:
	virtual void BeginPlay() override;

	//Hex Tiles
	virtual void ConstructHexagon();
	void CreateTile(const FVector& Location, float Scale);
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHexTile> HexTileClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* HexTileMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TileScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TileSpacing = 0.f; //Space between tiles
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int BigHexagonRadius = 2; //Radius in number of tiles

	float LongRadius;
	float GetMeshRadius() const;
	
	//Robots
	void SpawnRobot();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AConstructionRobot> ConstructionRobotClass;
	
	
private:

	UPROPERTY()
	TArray<AHexTile*> HexTiles;
	UPROPERTY()
	TArray<AConstructionRobot*> ConstructionRobots;
	UPROPERTY()
	AConstructionRobot* ConstructionRobot;
	

};

