// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structures.generated.h"

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
struct FNeighbourData
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EDirection> Direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UClass*> PossibleRoomsInDir;
};