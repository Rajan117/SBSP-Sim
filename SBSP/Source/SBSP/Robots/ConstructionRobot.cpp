// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionRobot.h"

#include "Kismet/KismetSystemLibrary.h"
#include "SBSP/HexGrid/HexGrid.h"
#include "SBSP/HexGrid/HexTile.h"


AConstructionRobot::AConstructionRobot()
{
	PrimaryActorTick.bCanEverTick = true;
	RobotState = ERobotState::Free;
}

void AConstructionRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToTarget(DeltaTime);
}

void AConstructionRobot::SpawnInit(AHexGrid* SpawningHarbour)
{
	HarbourRef = SpawningHarbour;
	HarbourLocation = HarbourRef->GetActorLocation();
	HarbourLocation.Z += HarbourRef->GetTileHeight();
	HexTileClass = HarbourRef->GetTileClass();
	RequestTile();
}

void AConstructionRobot::BeginPlay()
{
	Super::BeginPlay();
}

void AConstructionRobot::PlaceTileAtLocation(const FVector& Location)
{
	if (RobotState != ERobotState::Free) return;
	RobotState = ERobotState::MovingTile;
	TargetLocation = Location;
}

void AConstructionRobot::PlaceTile()
{
	if (!HexTileClass) return;
	
	if (AHexTile* SpawnedTile = Cast<AHexTile>(GetWorld()->SpawnActor(
		HexTileClass,
		&TargetLocation
	)))
	{
		RobotState = ERobotState::ReturningHome;
		TargetLocation = HarbourLocation;
	}
}

void AConstructionRobot::RequestTile()
{
	if (!HarbourRef) return;

	FVector NewLocation = FVector::ZeroVector;
	if (HarbourRef->GetNextTile(NewLocation))
	{
		RobotState = ERobotState::MovingTile;
		TargetLocation = NewLocation;
	}
}

void AConstructionRobot::MoveToTarget(const float DeltaTime)
{
	if (!HarbourRef || !CanMove()) return;
	if (RobotState == ERobotState::Free) return;
	
	FVector NewTargetLocation = TargetLocation;
	NewTargetLocation.Z = NewTargetLocation.Z+75;
	const FVector NewLocation = FMath::VInterpTo(     
		GetActorLocation(),
		NewTargetLocation,
		DeltaTime,
		RobotSpeed);
	SetActorLocation(NewLocation);
	
	if (RobotState == ERobotState::MovingTile &&
		FVector::DistXY(GetActorLocation(), TargetLocation) <= HarbourRef->GetMeshRadius()+HarbourRef->GetTileSpacing())
	{
		PlaceTile();
	}
	else if (RobotState == ERobotState::ReturningHome &&
		FVector::DistXY(GetActorLocation(), HarbourLocation) <= HarbourRef->GetMeshRadius()+HarbourRef->GetTileSpacing())
	{
		RobotState = ERobotState::Free;
		RequestTile();
	}
}

bool AConstructionRobot::CanMove()
{
	const FVector Start = GetActorLocation();
	FVector End = Start;
	End.Z = End.Z-100;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		CollisionParams
	);

	//Debug Line.
	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		HitResult.bBlockingHit ? FColor::Green : FColor::Red,
		false,
		0.1f,
		0,
		5.0f
	);
	
	return HitResult.bBlockingHit;
}


