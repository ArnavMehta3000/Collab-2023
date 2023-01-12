// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structures.h"
#include "RoomBase.h"
#include "Generator.generated.h"

UCLASS()
class UNTITLEDSTEALTHGAME_API AGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Generate(ARoomBase* startRoom);

	UFUNCTION(BlueprintCallable)
	void GenerateIterative();

	UFUNCTION(BlueprintCallable)
	void CloseLastRoom ();


	UFUNCTION(BlueprintPure)
	inline TArray<ARoomBase*>& GetAllSpawnedRooms() { return spawnedRooms; }

	UFUNCTION(BlueprintCallable)
	ARoomBase* SpawnRoom(TSubclassOf<ARoomBase> startRoom, FTransform transform);

private:
	inline ARoomBase* SpawnRoom(TSubclassOf<ARoomBase> startRoom, FVector position) { return SpawnRoom(startRoom, FTransform(position)); }
	FNeighbourData GetRandomNeighbourData(ARoomBase* room);
	bool NextDirectionIsValid(EDirection dir);
	FVector MoveInDirection(EDirection dir, FVector vec);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 spawnCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 iterationLimit = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float gridCellSize = 1000.0f;

private:
	TArray<ARoomBase*> spawnedRooms;
	FVector spawnPointer;
	int32 depth = 0;
};
