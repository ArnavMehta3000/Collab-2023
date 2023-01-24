// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structures.h"
#include "RoomBase.generated.h"


UCLASS()
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
	void SetParent(ARoomBase* room) { ParentRoom = room; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 MaxEnemySpawnCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 MaxGadgetSpawnCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 MaxKeySpawnCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FNeighbourData> NeighboursData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FNeighbourData> ClosingData;

private:
	ARoomBase* ParentRoom;
};
