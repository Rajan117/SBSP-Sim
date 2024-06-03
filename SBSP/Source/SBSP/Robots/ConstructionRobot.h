// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConstructionRobot.generated.h"

class AHexTile;

UENUM(BlueprintType)
enum class ERobotState : uint8
{
	Free UMETA(DisplayName = "Free"),
	ReturningHome UMETA(DisplayName = "Returning Home"),
	MovingTile UMETA(DisplayName = "Moving Tile"),
	None UMETA(Hidden), 
	MAX UMETA(Hidden), 
};

UCLASS()
class SBSP_API AConstructionRobot : public AActor
{
	GENERATED_BODY()

public:
	AConstructionRobot();

	virtual void Tick(float DeltaTime) override;

	virtual void PlaceTileAtLocation(const FVector& Location);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* RobotMesh;
	UPROPERTY(EditDefaultsOnly)
	float RobotSpeed = 1.f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHexTile> HexTileClass;

	virtual void PlaceTile();
	
private:
	ERobotState RobotState = ERobotState::Free;
	FVector HarbourLocation;
	FVector TargetLocation;
	void MoveToTarget(float DeltaTime);

public:
	FORCEINLINE void SetHexTileClass(TSubclassOf<AHexTile> InHexTileClass) { HexTileClass = InHexTileClass; }
	FORCEINLINE void SetHarbourLocation(const FVector& Location) { HarbourLocation = Location; }
	FORCEINLINE void SetTargetLocation(const FVector& Location) { TargetLocation = Location; }
	FORCEINLINE ERobotState GetRobotState() const { return RobotState; }
};
