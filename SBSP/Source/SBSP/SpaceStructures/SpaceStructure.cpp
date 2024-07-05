// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceStructure.h"

#include "Kismet/KismetMathLibrary.h"
#include "SBSP/HexGrid/HexGrid.h"

ASpaceStructure::ASpaceStructure()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpaceStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpaceStructure::BeginPlay()
{
	Super::BeginPlay();
	LongRadius = (GetMeshRadius()+10)*(HarbourTileRadius*2+1);
	GenerateHarbourLocations();
}

float ASpaceStructure::GetMeshRadius() const
{
	if (!HexTileMesh) return -1.f;
	const float MaxX = HexTileMesh->GetBoundingBox().Max.X;
	return 1.f*MaxX;
}

void ASpaceStructure::GenerateHarbourLocations()
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
	
	const float HexSide = LongRadius;
	const int NumSpawns = SpawnScheme.Num();
	
	for (int mult = 0; mult <= RadiusInHarbours; mult++)
	{
		int hn = 0;
		for (int j = 0; j < NumSpawns; j++)
		{
			for (int i = 0; i < mult; i++, hn++)
			{
				const FVector Location = FVector(CurrentPoint.X, CurrentPoint.Y, CurrentPoint.Z);
				SpawnHarbour(Location);
				CurrentPoint += (SpawnScheme[j]*HexSide);
			}
			if (j==4)
			{
				const FVector Location = FVector(CurrentPoint.X, CurrentPoint.Y, CurrentPoint.Z);
				SpawnHarbour(Location);
				CurrentPoint += (SpawnScheme[j]*HexSide);
				hn++;
				if (mult==RadiusInHarbours) break;
			}
		}
		
	}
}

void ASpaceStructure::SpawnHarbour(const FVector& SpawnLocation)
{
	if (HarbourClass)
	{
		if (AHexGrid* SpawnedHarbour = Cast<AHexGrid>(GetWorld()->SpawnActor(
		HarbourClass,
		&SpawnLocation
		)))
		{
			SpawnedHarbour->SpawnInit(this, HarbourTileRadius);
			Harbours.Add(SpawnedHarbour);
		}
	}
}



