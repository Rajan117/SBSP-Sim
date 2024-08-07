// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGrid.h"

#include "PrimitiveSceneInfo.h"
#include "SAdvancedTransformInputBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SBSP/HexGrid/HexTile.h"
#include "SBSP/Robots/ConstructionRobot.h"
#include "SBSP/Rockets/Rocket.h"
#include "SBSP/Controllers/SBSPPlayerController.h"


AHexGrid::AHexGrid()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	RootComponent = SceneRootComponent;
}

void AHexGrid::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AHexGrid::SpawnInit(ASpaceStructure* InSpaceStructure, FSimSettings InSimSettings)
{
	SpaceStructure = InSpaceStructure;
	SimSettings = InSimSettings;
	CurrentTileStock = InSimSettings.InitStock;
	GenerateTileLocations();
	ConstructTiles();
	InitiateRestockingTimer();
	SpawnRobots();
}

void AHexGrid::BeginPlay()
{
	Super::BeginPlay();
	LongRadius = GetMeshRadius();
}

void AHexGrid::Restock(int32 AddedStock)
{
	if (!GetShouldRestock()) return;
	CurrentTileStock += AddedStock;
	AddTiles(AddedStock);
	OnHarbourRestockedDelegate.Broadcast();
}

void AHexGrid::AddTiles(int32 NewStock)
{
	if (!HexTileClass) return;
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 50;
	
	for (int i = CurrentTileStock; i <= CurrentTileStock+NewStock; i++)
	{
		SpawnLocation.Z += 1*i;
		if (AHexTile* SpawnedTile = Cast<AHexTile>(GetWorld()->SpawnActor(
			HexTileClass,
			&SpawnLocation
		)))
		{
			SpawnedTile->SetActorRotation(GetActorRotation());
			HexTiles.Add(SpawnedTile);
		}
	}
}

void AHexGrid::ConstructTiles()
{
	if (!HexTileClass) return;
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 100;
	
	for (int i = 1; i <= CurrentTileStock; i++)
	{
		SpawnLocation.Z += 1*i;
		if (AHexTile* SpawnedTile = Cast<AHexTile>(GetWorld()->SpawnActor(
			HexTileClass,
			&SpawnLocation
		)))
		{
			HexTiles.Add(SpawnedTile);
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
	
	for (int mult = 0; mult <= SimSettings.TileRadius; mult++)
	{
		int hn = 0;
		for (int j = 0; j < NumSpawns; j++)
		{
			for (int i = 0; i < mult; i++, hn++)
			{
				const FVector Location = FVector(CurrentPoint.X, CurrentPoint.Y, CurrentPoint.Z);
				TileLocations.Enqueue(Location);
				RequiredTiles++;
				CurrentPoint += GetActorRotation().RotateVector(SpawnScheme[j] * HexSide);
			}
			if (j==4)
			{
				const FVector Location = FVector(CurrentPoint.X, CurrentPoint.Y, CurrentPoint.Z);
				TileLocations.Enqueue(Location);
				RequiredTiles++;
				CurrentPoint += GetActorRotation().RotateVector(SpawnScheme[j] * HexSide);
				hn++;
				if (mult==SimSettings.TileRadius) break;
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
	if (!TileLocations.IsEmpty())
	{
		if (CurrentTileStock > 0)
		{
			TileLocations.Dequeue(InLocation);
			if (TileLocations.IsEmpty())
			{
				bIsComplete = true;
				OnHarbourCompleteDelegate.Broadcast(
					RequiredTiles,
					ConstructionRobots.Num(),
					RocketRef->GetNumLaunches(),
					GetTotalDistanceRobotsTravelled()
				);
			}
			CurrentTileStock--;
			if (!HexTiles.IsEmpty())
			{
				if (AHexTile* HexTile = HexTiles.Pop()) HexTile->Destroy();
			}
			return true;
		}
		return false;
	}
	return false;
}

#pragma endregion 

#pragma region Robots

void AHexGrid::SpawnRobots()
{
	if (!ConstructionRobotClass) return;
	FVector Location = GetActorLocation();
	Location.Z += (HexTileMesh->GetBoundingBox().Max.Z*TileScale);

	for (int i = 0; i < SimSettings.NumRobots; i++)
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

void AHexGrid::InitiateRestockingTimer()
{
	FVector Location = GetActorLocation();
	if (ARocket* SpawnedRocket = Cast<ARocket>(GetWorld()->SpawnActor(
	RocketClass,
	&Location
	)))
	{
		SpawnedRocket->SpawnInit(this, Location);
		RocketRef = SpawnedRocket;
	}
}



bool AHexGrid::IsRobotFree(const AConstructionRobot* Robot)
{
	return Robot->GetRobotState() == ERobotState::Free;

}

#pragma endregion 

bool AHexGrid::GetShouldRestock() const
{
	if (CurrentTileStock < RequiredTiles && !TileLocations.IsEmpty())
	{
		return true;
	}
	return false;
}

float AHexGrid::GetTotalDistanceRobotsTravelled()
{
	float TotalDistanceTravelled = 0;
	for (AConstructionRobot* ConstructionRobot : ConstructionRobots)
	{
		if (!ConstructionRobot) continue;
		TotalDistanceTravelled += ConstructionRobot->GetDistanceTravelled();
	}
	return TotalDistanceTravelled;
}
