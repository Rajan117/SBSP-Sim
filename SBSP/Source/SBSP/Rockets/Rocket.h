// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rocket.generated.h"

class AHexGrid;

UCLASS()
class SBSP_API ARocket : public AActor
{
	GENERATED_BODY()

public:
	ARocket();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	void SpawnInit(AHexGrid* Harbour,
		const FVector& InStartLocation,
		const FVector& InTargetLocation);

protected:
	void Launch();
	void Dock();

private:
	FVector StartLocation;
	FVector TargetLocation;
	void MoveToTarget(float DeltaTime);

	FTimerHandle RestockTimerHandle;
	void Restart();

	UPROPERTY()
	AHexGrid* HarbourRef;
	
public:
	FORCEINLINE void SetTargetLocation(const FVector& InLocation) { TargetLocation = InLocation; }
	FORCEINLINE void SetStartLocation(const FVector& InLocation) { StartLocation = InLocation; }
};
