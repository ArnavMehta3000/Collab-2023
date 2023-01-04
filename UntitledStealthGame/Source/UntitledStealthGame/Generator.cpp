// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

// Sets default values
AGenerator::AGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	spawnPointer = FVector(0.0f, 0.0f, 0.0f);
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

void AGenerator::Generate(ARoomBase* startRoom)
{
	if (depth > spawnCount)
		return;
	else
		depth++;

	auto neighbourData = GetRandomNeighbourData(startRoom);
	int limit = 100;
	bool found = false;
	
	// Begin loop to get valid direction
	UClass* randRoom = nullptr;
	do
	{
		int index = FMath::RandRange(0, neighbourData.PossibleRoomsInDir.Num() - 1);
		if (NextDirectionIsValid(neighbourData.Direction))
		{
			// Next direction is valid, get a room in that direction
			randRoom = neighbourData.PossibleRoomsInDir[index];
			spawnPointer = MoveInDirection(neighbourData.Direction, spawnPointer);
			found = true;
		}
		else
		{
			neighbourData = GetRandomNeighbourData(startRoom);
		}

		limit--;
	} while (limit > 0 && !found);
	// End loop

	if (randRoom != nullptr)
		Generate(SpawnRoom(randRoom, spawnPointer));
	else
	{
		depth--;
		Generate(startRoom);
	}
}

void AGenerator::CloseLastRoom()
{
}

ARoomBase* AGenerator::SpawnRoom(TSubclassOf<ARoomBase> startRoom, FTransform transform)
{
	auto room = GetWorld()->SpawnActor<ARoomBase>(startRoom, transform);
	spawnedRooms.Push(room);
	return room;
}

FNeighbourData AGenerator::GetRandomNeighbourData(ARoomBase* room)
{
	int index = FMath::RandRange(0, room->NeighboursData.Num() - 1);
	return room->NeighboursData[index];	
}

bool AGenerator::NextDirectionIsValid(EDirection dir)
{
	FVector temp = MoveInDirection(dir, spawnPointer);

	for (auto& room : spawnedRooms)
		if (temp.Equals(room->GetActorLocation()))
			return false;

	return true;
}

FVector AGenerator::MoveInDirection(EDirection dir, FVector vec)
{
	FVector temp = vec;

	switch (dir)
	{
	case PosX:
		temp.X += gridCellSize;
		break;
	case PosY:
		temp.Y += gridCellSize;
		break;
	case NegX:
		temp.X -= gridCellSize;
		break;
	case NegY:
		temp.Y -= gridCellSize;
		break;
	default:
		checkNoEntry();
	}

	return temp;
}
