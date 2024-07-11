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

	UFUNCTION()
	void Launch();
	void SpawnInit(AHexGrid* Harbour,
		const FVector& InTargetLocation);

protected:
	void Dock();

	UPROPERTY(EditDefaultsOnly)
	float RocketSpeed = 1.f;

	//Payload
	UPROPERTY(EditDefaultsOnly, Category = Payload)
	int32 NumRobots = 0;
	UPROPERTY(EditDefaultsOnly, Category = Payload)
	int32 NumTiles = 20;

private:
	FVector StartLocation;
	FVector TargetLocation;
	void MoveToTarget(float DeltaTime);
	bool bCanMove = false;

	FTimerHandle DockingTimerHandle;
	float DockingDelay = 5.f;
	FTimerHandle RestockTimerHandle;
	float LaunchFrequency = 15.f;
	UFUNCTION()
	void Restart();
	int32 NumLaunches = 0;


	UPROPERTY()
	AHexGrid* HarbourRef;
	
public:
	FORCEINLINE void SetTargetLocation(const FVector& InLocation) { TargetLocation = InLocation; }
	FORCEINLINE void SetStartLocation(const FVector& InLocation) { StartLocation = InLocation; }
	FORCEINLINE int32 GetNumLaunches() const { return NumLaunches; }
};
