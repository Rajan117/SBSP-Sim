// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConstructionRobot.generated.h"

class AHexGrid;
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

	void SpawnInit(AHexGrid* SpawningHarbour);
	virtual void PlaceTileAtLocation(const FVector& Location);
	void RequestTile();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneRootComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* RobotMesh;
	UPROPERTY(EditDefaultsOnly)
	float RobotSpeed = 1.f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHexTile> HexTileClass;

	virtual void PlaceTile();
	
private:
	//Harbour
	UPROPERTY()
	AHexGrid* HarbourRef;
	FVector HarbourLocation;

	//Movement
	ERobotState RobotState = ERobotState::Free;
	FVector TargetLocation;
	void MoveToTarget(float DeltaTime);
	bool CanMove();
	void PointToLocation(const FVector& Location);

	//Tile
	UPROPERTY()
	AHexTile* HexTileRef;
	void SpawnTile();
	void DespawnTile();

public:
	FORCEINLINE void SetHexTileClass(TSubclassOf<AHexTile> InHexTileClass) { HexTileClass = InHexTileClass; }
	FORCEINLINE void SetHarbourLocation(const FVector& Location) { HarbourLocation = Location; }
	FORCEINLINE void SetTargetLocation(const FVector& Location) { TargetLocation = Location; }
	FORCEINLINE void SetHarbour(AHexGrid* InHarbourRef) { HarbourRef = InHarbourRef; }
	FORCEINLINE ERobotState GetRobotState() const { return RobotState; }
};
