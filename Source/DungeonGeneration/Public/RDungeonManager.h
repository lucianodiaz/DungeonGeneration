// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RDungeonManager.generated.h"

class ARoom;
class ARTiles;

USTRUCT(BlueprintType)
struct FRoomStruct
{
	GENERATED_BODY()

	FRoomStruct()
	{
		ID = 0;
		Position = FVector2D::Zero();
		SizeX =0;
		SizeY =0;
		CreatedByID =0;
		Direction=0;
	}
	UPROPERTY(VisibleAnywhere)
	int ID;

	UPROPERTY(VisibleAnywhere)
	int CreatedByID;

	UPROPERTY(VisibleAnywhere)
	int Direction;
	
	UPROPERTY(VisibleAnywhere)
	FVector2D Position;
	
	UPROPERTY(VisibleAnywhere)
	int SizeX;

	UPROPERTY(VisibleAnywhere)
	int SizeY;
	
	UPROPERTY(VisibleAnywhere)
	ARTiles* TileContainer;
	
	int Size() const
	{
		return SizeX*SizeY;
	}
};

UCLASS()
class DUNGEONGENERATION_API ARDungeonManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARDungeonManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateFirstRoom();

	void CreateRooms();

	void GenerateRoomAdjacent(const FRoomStruct& BaseRoom, FRoomStruct& NewRoom);
	
	void AddRoomToList(const FRoomStruct& Room);

	void CreateFloor(FRoomStruct& Room);

	void CreateWalls(FRoomStruct& Room);

	bool DoesCollide(const FRoomStruct& RoomA, const FRoomStruct& RoomB) const;

	void RandomSize();
	FRoomStruct& GetRandomRoom();
	///VARIABLES
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(ClampMin=5))
	int SizeX;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(ClampMin=5))
	int SizeY;

	UPROPERTY(BlueprintReadOnly)
	int TileSize;

	UPROPERTY(BlueprintReadWrite)
	TArray<FRoomStruct> Rooms;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<ARTiles> TileClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<ARoom> RoomClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UStaticMesh* WallMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int RoomsQuantity = 10;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
