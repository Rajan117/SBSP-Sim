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
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	RootComponent = SceneRootComponent;
}

void AHexGrid::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AHexGrid::BeginPlay()
{
	Super::BeginPlay();
	CurrentTileStock = InitialTileStock;
	LongRadius = GetMeshRadius();
	GenerateTileLocations();
	SpawnRobots();
	InitiateRestockingTimer();
	ConstructTiles();
}

void AHexGrid::Restock()
{
	UKismetSystemLibrary::PrintString(this, "Restocked");
	AddTiles(RestockAmount);
	CurrentTileStock += RestockAmount;
	OnHarbourRestockedDelegate.Broadcast();
}

void AHexGrid::AddTiles(int32 NewStock)
{
	if (!HexTileClass || !HexTileMesh) return;
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 50;
	
	for (int i = CurrentTileStock; i <= CurrentTileStock+NewStock; i++)
	{
		SpawnLocation.Z += 5.f*i;
		if (AHexTile* SpawnedTile = Cast<AHexTile>(GetWorld()->SpawnActor(
			HexTileClass,
			&SpawnLocation
		)))
		{
			HexTiles.Add(SpawnedTile);
		}
	}
}

void AHexGrid::ConstructTiles()
{
	if (!HexTileClass || !HexTileMesh) return;
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 50;
	
	for (int i = 1; i <= CurrentTileStock; i++)
	{
		SpawnLocation.Z += 5.f*i;
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
		if (!HexTiles.IsEmpty())
		{
			if (AHexTile* HexTile = HexTiles.Pop()) HexTile->Destroy();
		}
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

void AHexGrid::InitiateRestockingTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		RestockTimerHandle,
		this,
		&AHexGrid::Restock,
		RestockFrequency,
		true
	);
}

bool AHexGrid::IsRobotFree(const AConstructionRobot* Robot)
{
	return Robot->GetRobotState() == ERobotState::Free;

}

#pragma endregion 
