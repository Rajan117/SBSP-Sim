// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"

#include "Kismet/KismetSystemLibrary.h"
#include "SBSP/HexGrid/HexGrid.h"


ARocket::ARocket()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToTarget(DeltaTime);
}

void ARocket::BeginPlay()
{
	bCanMove = false;
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	GetWorld()->GetTimerManager().SetTimer(
		DockingTimerHandle,
		this,
		&ARocket::Launch,
		DockingDelay,
		true
	);
}

void ARocket::SpawnInit(AHexGrid* Harbour, const FVector& InTargetLocation)
{
	if (Harbour) HarbourRef = Harbour;
	FVector NewStartLocation = InTargetLocation;
	NewStartLocation.Z -= 2000;
	StartLocation = NewStartLocation;
	TargetLocation = InTargetLocation;
	SetActorLocation(StartLocation);
}

void ARocket::Launch()
{
	NumLaunches++;
	SetActorLocation(StartLocation);
	SetActorHiddenInGame(false);
	bCanMove = true;
}

void ARocket::Dock()
{
	if (!HarbourRef) return;
	bCanMove = false;
	HarbourRef->Restock(NumTiles);
	
	GetWorld()->GetTimerManager().SetTimer(
	DockingTimerHandle,
	this,
	&ARocket::Restart,
	DockingDelay,
	true
	);
}

void ARocket::MoveToTarget(float DeltaTime)
{
	if (!HarbourRef || !bCanMove) return;

	const FVector NewLocation = FMath::VInterpTo(     
		GetActorLocation(),
		TargetLocation,
		DeltaTime,
		RocketSpeed);
	SetActorLocation(NewLocation);
	
	if (FVector::Distance(GetActorLocation(), TargetLocation) < 250)
	{
		Dock();
	}
}

void ARocket::Restart()
{
	SetActorLocation(StartLocation);
	SetActorHiddenInGame(true);
	
	GetWorld()->GetTimerManager().SetTimer(
	DockingTimerHandle,
	this,
	&ARocket::Launch,
	DockingDelay,
	true
	);
}



