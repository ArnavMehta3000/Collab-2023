// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomBase.generated.h"

class ARoomBase;
enum EDirection;
struct FDirectionalNeighbours;


UCLASS(BlueprintType)
class UNTITLEDSTEALTHGAME_API ARoomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetCameFromDirection(EDirection dir) { cameFromDirection = dir; }


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDirectionalNeighbours> NeighbourList;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDirectionalNeighbours> ClosingRoomsList;

private:
	EDirection cameFromDirection;
	ARoomBase* parentRoom = nullptr;
};

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
