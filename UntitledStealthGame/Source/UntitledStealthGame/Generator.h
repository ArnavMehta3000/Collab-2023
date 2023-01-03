// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomNeighbourConnections.h"
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

	UFUNCTION(BlueprintPure)
	ARoomBase* GetFirstRoom();


	UFUNCTION(BlueprintPure)
	ARoomBase* GetLastRoom();


	UFUNCTION(BlueprintPure)
	int32 GetSpawnedRoomCount();


	UFUNCTION(BlueprintCallable)
	void ClearAllRooms();


	UFUNCTION(BlueprintCallable)
	void Generate(ARoomBase* room);


	UFUNCTION(BlueprintCallable)
	ARoomBase* SpawnRoom(TSubclassOf<ARoomBase> startRoom, FVector position);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float cellSize = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URoomNeighbourConnections* rooomsDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 roomCount = 5;

private:
	TArray<ARoomBase*> spawnedRooms;
	FVector spawnPointer;
};
