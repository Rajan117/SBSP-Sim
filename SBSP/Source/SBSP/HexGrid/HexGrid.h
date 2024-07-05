// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ChildActorComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "HexGrid.generated.h"

class ASpaceStructure;
class ARocket;
class AHexTile;
class AConstructionRobot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHarbourRestocked);

UCLASS()
class SBSP_API AHexGrid : public AActor
{
	GENERATED_BODY()

public:
	AHexGrid();
	virtual void Tick(float DeltaSeconds) override;

	void SpawnInit(ASpaceStructure* InSpaceStructure, int32 HarbourTileRadius);
	float GetMeshRadius() const;
	bool GetNextTile(FVector& InLocation);
	void Restock(int32 AddedStock);
	static bool IsRobotFree(const AConstructionRobot* Robot);
	FOnHarbourRestocked OnHarbourRestockedDelegate;

protected:
	virtual void BeginPlay() override;

	//Hex Tiles
	void AddTiles(int32 NewStock);
	virtual void ConstructTiles();
	virtual void GenerateTileLocations();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHexTile> HexTileClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* HexTileMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TileScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TileSpacing = 0.f; //Space between tiles
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BigHexagonRadius = 2; //Radius in number of tiles
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InitialTileStock = 20; //Initial number of tiles harbour has
	float LongRadius;
	
	//Robots
	void SpawnRobots();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AConstructionRobot> ConstructionRobotClass;
	UPROPERTY(EditDefaultsOnly)
	int32 NumberOfRobots = 1;

	//Restocking
	UPROPERTY(EditDefaultsOnly)
	int32 RestockAmount = 10;
	UPROPERTY(EditDefaultsOnly)
	float RestockFrequency = 10; //Number of seconds between restocks
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARocket> RocketClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneRootComponent;
	
private:
	int32 CurrentTileStock = 0;
	TArray<AHexTile*> HexTiles;
	TQueue<FVector> TileLocations;
	int32 RequiredTiles = 0;
	UPROPERTY()
	TArray<AConstructionRobot*> ConstructionRobots;
	UPROPERTY()
	ASpaceStructure* SpaceStructure;
	
	void InitiateRestockingTimer();
	FTimerHandle RestockTimerHandle;
	UPROPERTY()
	ARocket* RocketRef;
	
public:
	FORCEINLINE TSubclassOf<AHexTile> GetTileClass() const { return HexTileClass; }
	FORCEINLINE float GetTileSpacing() const { return TileSpacing; }
	FORCEINLINE float GetTileHeight() const { return HexTileMesh->GetBoundingBox().Max.Z*TileScale; }
	FORCEINLINE void SetSpaceStructure(ASpaceStructure* InSpaceStructure) { SpaceStructure = InSpaceStructure; }
};

