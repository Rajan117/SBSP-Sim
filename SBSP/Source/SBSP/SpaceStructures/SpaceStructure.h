// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceStructure.generated.h"

class AHexGrid;

UCLASS()
class SBSP_API ASpaceStructure : public AActor
{
	GENERATED_BODY()

public:
	ASpaceStructure();
	virtual void Tick(float DeltaTime) override;

	float GetMeshRadius() const;


protected:
	virtual void BeginPlay() override;

	void GenerateHarbourLocations();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* HexTileMesh;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHexGrid> HarbourClass;
	UPROPERTY(EditDefaultsOnly)
	int32 HarbourTileRadius = 6;
	UPROPERTY(EditDefaultsOnly)
	int32 RadiusInHarbours = 2;

private:
	UPROPERTY()
	TArray<AHexGrid*> Harbours;
	float LongRadius;
	void SpawnHarbour(const FVector& SpawnLocation);
};
