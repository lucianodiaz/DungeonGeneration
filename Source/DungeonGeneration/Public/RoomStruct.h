// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RoomStruct.generated.h"

/**
 * 
 */


class ARDoor;
class ARTiles;
class ARoom;

UENUM()
enum ERoomType
{
	
	Boss UMETA(DisplayName = "Boos Room"),
	Trap UMETA(DisplayName = "Trap Room"),
	Treasure UMETA(DisplayName = "Treasure Room"),
	// Shop UMETA(DisplayName = "Shop Room"),
	Normal UMETA(DisplayName = "Normal Room")
};

USTRUCT(BlueprintType)
struct FRoomStruct
{
	GENERATED_BODY()

	FRoomStruct(): RoomActor(nullptr)
	{
		TileContainer = nullptr;
		ID = 0;
		Position = FVector2D::Zero();
		SizeX = 0;
		SizeY = 0;
		CreatedByID = 0;
		Direction = 0;
		RoomType = Normal;
	}

	UPROPERTY(VisibleAnywhere)
	int ID;

	UPROPERTY(VisibleAnywhere)
	int CreatedByID;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int Direction;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector2D Position;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int SizeX;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int SizeY;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	ARTiles* TileContainer;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<ARDoor*> Doors;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	ARoom* RoomActor;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TEnumAsByte<ERoomType> RoomType = Normal;
	// BlueprintCallable function
	// UFUNCTION(BlueprintCallable, Category="Room")
	int Size() const
	{
		return SizeX*SizeY;
	}
};

