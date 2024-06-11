// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionRobot.h"

#include "Kismet/KismetSystemLibrary.h"
#include "SBSP/HexGrid/HexGrid.h"
#include "SBSP/HexGrid/HexTile.h"


AConstructionRobot::AConstructionRobot()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AConstructionRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToTarget(DeltaTime);
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
	UKismetSystemLibrary::PrintString(this, TargetLocation.ToString());
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
		UKismetSystemLibrary::PrintString(this, "Placed Tile");
	}
}

void AConstructionRobot::MoveToTarget(const float DeltaTime)
{
	if (!HarbourRef || !CanMove()) return;
	
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
		UKismetSystemLibrary::PrintString(this, "Placing Tile");
		PlaceTile();
	}
	else if (RobotState == ERobotState::ReturningHome &&
		FVector::DistXY(GetActorLocation(), HarbourLocation) < 5.f)
	{
		UKismetSystemLibrary::PrintString(this, "Returned Home");
		RobotState = ERobotState::Free;
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


