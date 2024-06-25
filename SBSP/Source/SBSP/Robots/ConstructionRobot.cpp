// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionRobot.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SBSP/HexGrid/HexGrid.h"
#include "SBSP/HexGrid/HexTile.h"


AConstructionRobot::AConstructionRobot()
{
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	RootComponent = SceneRootComponent;
	
	RobotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Robot Mesh"));
	RobotMesh->SetupAttachment(RootComponent);
	
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
	if (!SpawningHarbour) return;
	
	HarbourRef = SpawningHarbour;
	HarbourLocation = HarbourRef->GetActorLocation();
	HarbourLocation.Z += HarbourRef->GetTileHeight();
	HexTileClass = HarbourRef->GetTileClass();
	HarbourRef->OnHarbourRestockedDelegate.AddDynamic(this, &AConstructionRobot::OnHarbourRestocked);
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
		DespawnTile();
		RobotState = ERobotState::ReturningHome;
		TargetLocation = HarbourLocation;
	}
}

void AConstructionRobot::OnHarbourRestocked()
{
	if (!HarbourRef || RobotState != ERobotState::Free) return;
	RequestTile();
}

void AConstructionRobot::RequestTile()
{
	if (!HarbourRef || RobotState != ERobotState::Free) return;

	FVector NewLocation = FVector::ZeroVector;
	if (HarbourRef->GetNextTile(NewLocation))
	{
		SpawnTile();
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
	PointToLocation(TargetLocation);
	
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
	End.Z = End.Z-200;

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

void AConstructionRobot::PointToLocation(const FVector& Location)
{
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	const FRotator CurrentRotation = GetActorRotation();
	const FRotator NewRotation(CurrentRotation.Pitch, TargetRotation.Yaw, CurrentRotation.Roll);	
	SetActorRotation(NewRotation);
}

void AConstructionRobot::SpawnTile()
{
	if (HexTileRef || !HexTileClass || !HarbourRef || !RobotMesh) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	const FVector SpawnLocation = RobotMesh->GetSocketLocation(FName("TileSocket"));
	const FRotator SpawnRotation = GetActorRotation();

	HexTileRef = GetWorld()->SpawnActor<AHexTile>(HexTileClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (HexTileRef)
	{
		HexTileRef->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AConstructionRobot::DespawnTile()
{
	if (HexTileRef)
	{
		HexTileRef->Destroy();
		HexTileRef = nullptr;
	}
}


