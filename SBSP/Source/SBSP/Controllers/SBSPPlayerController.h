// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SBSPPlayerController.generated.h"

class ASpaceStructure;
class ASBSPHUD;

UCLASS()
class SBSP_API ASBSPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASBSPPlayerController();
	virtual void BeginPlay() override;

protected:
	void SpawnStructure();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpaceStructure> SpaceStructureClass;
	UFUNCTION()
	void OnSpaceStructureCompleted(int32 NumTiles, int32 NumRobots, int32 NumLaunches, float TotalTime);
	


private:
	UPROPERTY()
	ASBSPHUD* SBSPHUD;
	UPROPERTY()
	ASpaceStructure* SpaceStructure;

public:
	FORCEINLINE ASpaceStructure* GetSpaceStructure() const { return  SpaceStructure; }

};
