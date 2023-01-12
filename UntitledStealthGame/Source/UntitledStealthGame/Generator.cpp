// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"
#include "UntitledStealthGame/DoorBase.h"

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
	int limit          = 100;
	bool found         = false;
	
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

void AGenerator::GenerateIterative()
{	
	UE_LOG(LogScript, Warning, TEXT("Starting iterative generation. Spawn Count: %u"), spawnCount);

	ARoomBase* currentRoom = nullptr;
	int currentLimit = 0;
	for (int i = 0; i < spawnCount + 1; i++)
	{
		currentRoom = spawnedRooms.Last();
		
		// Get random direction (from neightbour data)
		bool validDirFound = false;
		int neighbourDataIndex = -1;
		int iterationCount = 0;

		while (!validDirFound)
		{
			if (currentLimit >= iterationLimit)
			{
				// No valid room could be found remove previous room
				auto roomToDelete = spawnedRooms.Pop();
				roomToDelete->Destroy();

				i--;
				currentRoom        = spawnedRooms.Last();
				spawnPointer       = currentRoom->GetActorLocation();
				neighbourDataIndex = -1;
				validDirFound      = false;
				currentLimit       = 0;
			}


			int index = FMath::RandRange(0, currentRoom->NeighboursData.Num() - 1);
			auto& data = currentRoom->NeighboursData[index];

			if (NextDirectionIsValid(data.Direction))
			{
				// Next direction is valid, save info
				validDirFound = true;
				neighbourDataIndex = index;
			}
			else
			{
				// Keep trying until a valid direction is found
				iterationCount++;
				currentLimit++;
				continue;
			}
		}

		if (!validDirFound)
		{
			UE_LOG(LogScript, Error, TEXT("Valid room not found"));
			return;
		}


		
		UE_LOG(LogScript, Warning, TEXT("Room %u took %u iterations"), i, iterationCount);

		auto& neighbourData = currentRoom->NeighboursData[neighbourDataIndex];
		spawnPointer = MoveInDirection(neighbourData.Direction, spawnPointer);

		int randRoomIndex= FMath::RandRange(0, neighbourData.PossibleRoomsInDir.Num() - 1);
		SpawnRoom(neighbourData.PossibleRoomsInDir[randRoomIndex], spawnPointer);
	}
	UE_LOG(LogScript, Warning, TEXT("finished generation: Spawned Rooms: %u"), spawnedRooms.Num());
}

void AGenerator::CloseLastRoom()
{
	UE_LOG(LogScript, Warning, TEXT("Closing last room"));

	// Remove last room
	auto roomToDelete = spawnedRooms.Pop();
	roomToDelete->Destroy();

	// Get current room to close (prev room)
	ARoomBase* currentRoom = spawnedRooms.Last();
	spawnPointer = currentRoom->GetActorLocation();

	// Get closing data of cuurent room
	auto& dataArr = currentRoom->ClosingData;
	int validIndex = -1;

	for (int i = 0; i < dataArr.Num(); i++)
	{
		if (NextDirectionIsValid(dataArr[i].Direction))
		{
			validIndex = i;
			break;
		}
	}
	
	if (validIndex == -1)
	{
		UE_LOG(LogScript, Error, TEXT("Closing room not found"));
		return;
	}

	spawnPointer = MoveInDirection(dataArr[validIndex].Direction, spawnPointer);

	int randClosingRoomIndex = FMath::RandRange(0, dataArr[validIndex].PossibleRoomsInDir.Num() - 1);
	SpawnRoom(dataArr[validIndex].PossibleRoomsInDir[randClosingRoomIndex], spawnPointer);
}

void AGenerator::GenerateDoors(TSubclassOf<ADoorBase> doorClass)
{
	int limit = spawnedRooms.Num();

	for (int i = 1; i < limit; i++)
	{
		ARoomBase* currentRoom   = spawnedRooms[i];
		ARoomBase* prevRoom      = spawnedRooms[i - 1];
		FVector currentPos       = currentRoom->GetActorLocation();
		FVector prevPos          = prevRoom->GetActorLocation();
		FVector posDiff          = currentPos - prevPos;

		EDirection fromToDirection = GetDirectionFromPosDiff(posDiff);
		
		// Testing
		FString name = UEnum::GetValueAsString(fromToDirection);
		UE_LOG(LogScript, Warning, TEXT("From %u to %u. Direction: %s"), (i - 1), i, *name);

		FVector spawnPos = (currentPos + prevPos) / 2;
		FVector spawnRot(0.0f, 90.0f, 0.0f);
		// Place normally
		if (fromToDirection == PosY || fromToDirection == NegY)
		{
			auto door          = GetWorld()->SpawnActor<ADoorBase>(doorClass, FTransform(spawnPos));
			door->cameFromRoom = prevRoom;
			door->goingToRoom  = currentRoom;
		}
		// Rotate by 90 degrees
		if (fromToDirection == PosX || fromToDirection == NegX)
		{
			auto door          = GetWorld()->SpawnActor<ADoorBase>(doorClass, FTransform(spawnRot.Rotation(), spawnPos));
			door->cameFromRoom = prevRoom;
			door->goingToRoom  = currentRoom;
		}
	}
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

EDirection AGenerator::GetDirectionFromPosDiff(FVector diff)
{
	diff.Normalize();
	UE_LOG(LogScript, Warning, TEXT("Direction: (%f, %f)"), diff.X, diff.Y);

	FVector negX = FVector(-1.0f, 0.0f, 0.0f);
	FVector posX = FVector(1.0f, 0.0f, 0.0f);
	FVector negY = FVector(0.0f, -1.0f, 0.0f);
	FVector posY = FVector(0.0f, 1.0f, 0.0f);
	float tolerance = 0.1f;
	
	EDirection dir{};
	if (diff.Equals(negX, tolerance))
		dir = NegX;
	else if (diff.Equals(negY, tolerance))
		dir = NegY;
	else if (diff.Equals(posX, tolerance))
		dir = PosX;
	else if (diff.Equals(posY, tolerance))
		dir = PosY;
	else // Should never be the case
	{
		UE_LOG(LogScript, Error, TEXT("SHOULD NOT BE HERE Direction: (% f, % f)"), diff.X, diff.Y);
	}

	return dir;
}
