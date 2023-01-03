// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"
#include "UntitledStealthGame/RoomBase.h"
#include "UntitledStealthGame/RoomNeighbourConnections.h"

// Sets default values
AGenerator::AGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ARoomBase* AGenerator::GetFirstRoom()
{
	return nullptr;
}

ARoomBase* AGenerator::GetLastRoom()
{
	return nullptr;
}

int32 AGenerator::GetSpawnedRoomCount()
{
	return int32();
}

void AGenerator::ClearAllRooms()
{
}

void AGenerator::Generate(ARoomBase* room)
{
}

ARoomBase* AGenerator::SpawnRoom(TSubclassOf<ARoomBase> startRoom, FVector position)
{
	return nullptr;
}

