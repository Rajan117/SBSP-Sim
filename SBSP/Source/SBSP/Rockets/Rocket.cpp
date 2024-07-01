// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"


ARocket::ARocket()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARocket::BeginPlay()
{
	Super::BeginPlay();
}

void ARocket::SpawnInit(AHexGrid* Harbour, const FVector& InStartLocation, const FVector& InTargetLocation)
{
	if (Harbour) HarbourRef = Harbour;
	StartLocation = InStartLocation;
	TargetLocation = InTargetLocation;
}

void ARocket::Launch()
{
}

void ARocket::Dock()
{
	if (!HarbourRef) return;
}

void ARocket::MoveToTarget(float DeltaTime)
{
}

void ARocket::Restart()
{
}



