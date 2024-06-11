// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGrid.h"

#include "PrimitiveSceneInfo.h"
#include "SAdvancedTransformInputBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SBSP/HexGrid/HexTile.h"
#include "SBSP/Robots/ConstructionRobot.h"


AHexGrid::AHexGrid()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHexGrid::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//ConstructTiles();
}

void AHexGrid::BeginPlay()
{
	Super::BeginPlay();
	CurrentTileStock = InitialTileStock;
	LongRadius = GetMeshRadius();
	GenerateTileLocations();
	SpawnRobots();
}

void AHexGrid::ConstructTiles()
{
	if (TileLocations.IsEmpty()) return;
	
	TArray<AConstructionRobot*> FreeRobots = ConstructionRobots.FilterByPredicate(IsRobotFree);

	for (AConstructionRobot* Robot : FreeRobots)
	{
		if (Robot &&
		Robot->GetRobotState() == ERobotState::Free
		&& !TileLocations.IsEmpty()
		&& CurrentTileStock > 0)
		{
			FVector Location;
			TileLocations.Dequeue(Location);
			Robot->PlaceTileAtLocation(Location);
			CurrentTileStock--;
		}
	}
}

#pragma region Hex Tiles

void AHexGrid::GenerateTileLocations()
{
	const float Sqrt3 = UKismetMathLibrary::Sqrt(3);
	FVector CurrentPoint = GetActorLocation();

	TArray<FVector> SpawnScheme = {
		FVector(1.5f, -Sqrt3*0.5f, 0.0f),	//DR
		FVector(0.f, -Sqrt3, 0.f),			//DX
		FVector(-1.5f, -Sqrt3*0.5f, 0.f),	//DL
		FVector(-1.5f, Sqrt3*0.5f, 0.f),	//UL
		FVector(0.f, Sqrt3, 0.f),			//UX
		FVector(1.5f, Sqrt3*0.5f, 0.f),	//UR
	};
	
	const float HexSide = LongRadius + TileSpacing;
	const int NumSpawns = SpawnScheme.Num();
	
	for (int mult = 0; mult <= BigHexagonRadius; mult++)
	{
		int hn = 0;
		for (int j = 0; j < NumSpawns; j++)
		{
			for (int i = 0; i < mult; i++, hn++)
			{
				const FVector Location = FVector(CurrentPoint.X, CurrentPoint.Y, CurrentPoint.Z);
				TileLocations.Enqueue(Location);
				CurrentPoint += (SpawnScheme[j]*HexSide);
			}
			if (j==4)
			{
				const FVector Location = FVector(CurrentPoint.X, CurrentPoint.Y, CurrentPoint.Z);
				TileLocations.Enqueue(Location);
				CurrentPoint += (SpawnScheme[j]*HexSide);
				hn++;
				if (mult==BigHexagonRadius) break;
			}
		}
		
	}
}

float AHexGrid::GetMeshRadius() const
{
	if (!HexTileMesh) return -1.f;
	const float MaxX = HexTileMesh->GetBoundingBox().Max.X;
	return TileScale*MaxX;
}

bool AHexGrid::GetNextTile(FVector& InLocation)
{
	if (CurrentTileStock > 0 && !TileLocations.IsEmpty())
	{
		TileLocations.Dequeue(InLocation);
		CurrentTileStock--;
		return true;
	}
	return false;
}

#pragma endregion 

#pragma region Robots

void AHexGrid::SpawnRobots()
{
	if (!ConstructionRobotClass || !HexTileMesh) return;
	FVector Location = GetActorLocation();
	Location.Z += (HexTileMesh->GetBoundingBox().Max.Z*TileScale);

	for (int i = 0; i < NumberOfRobots; i++)
	{
		if (AConstructionRobot* SpawnedRobot = Cast<AConstructionRobot>(GetWorld()->SpawnActor(
		ConstructionRobotClass,
		&Location
		)))
		{
			SpawnedRobot->SpawnInit(this);
			ConstructionRobots.Add(SpawnedRobot);
		}
	}
}

bool AHexGrid::IsRobotFree(const AConstructionRobot* Robot)
{
	return Robot->GetRobotState() == ERobotState::Free;

}

#pragma endregion 
