// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoomNeighbourConnections.generated.h"

class ARoomBase;

UENUM(BlueprintType)
enum EDirection
{
	PosX UMETA(DisplayName = "+X"),
	PosY UMETA(DisplayName = "+Y"),
	NegX UMETA(DisplayName = "-X"),
	NegY UMETA(DisplayName = "-Y")
};

USTRUCT(BlueprintType)
struct FDirectionalNeighbours
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EDirection> Direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<ARoomBase>> PossibleRooms;
};

USTRUCT(BlueprintType)
struct FBaseRoomConnections
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARoomBase> BaseRoom;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDirectionalNeighbours> DirectionalNeighbours;
};


UCLASS()
class UNTITLEDSTEALTHGAME_API URoomNeighbourConnections : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FBaseRoomConnections> RoomConnections;
};
