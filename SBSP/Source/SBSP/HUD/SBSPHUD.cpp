// Fill out your copyright notice in the Description page of Project Settings.


#include "SBSPHUD.h"

#include "ResultsOverlay.h"
#include "SimOverlay.h"
#include "Kismet/KismetSystemLibrary.h"


ASBSPHUD::ASBSPHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASBSPHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ASBSPHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASBSPHUD::AddSimOverlay()
{
	if (SimOverlayClass)
	{
		SimOverlayRef = CreateWidget<USimOverlay>(GetOwningPlayerController(), SimOverlayClass);
		if (SimOverlayRef)
		{
			SimOverlayRef->AddToViewport();
		}
	}
}

void ASBSPHUD::RemoveSimOverlay()
{
	if (SimOverlayRef)
	{
		SimOverlayRef->RemoveFromParent();
		SimOverlayRef = nullptr;
	}
}

void ASBSPHUD::AddResultsOverlay(int32 NumTiles, int32 NumRobots, int32 NumLaunches, float TotalTime)
{
	if (ResultsOverlayClass)
	{
		ResultsOverlayRef = CreateWidget<UResultsOverlay>(GetOwningPlayerController(), ResultsOverlayClass);
		if (ResultsOverlayRef)
		{
			ResultsOverlayRef->SetResults(NumTiles, NumRobots, NumLaunches, TotalTime);
			ResultsOverlayRef->AddToViewport();
		}
	}
}

void ASBSPHUD::RemoveResultsOverlay()
{
	if (ResultsOverlayRef)
	{
		ResultsOverlayRef->RemoveFromParent();
		ResultsOverlayRef = nullptr;
	}
}

