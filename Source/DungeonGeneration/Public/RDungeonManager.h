// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RDungeonManager.generated.h"

class ARoom;
class ARTiles;
class ARDoor;

//For this project we will take this Way
// X it's going to take for Forward or maybe UP direction we don't use Z for UP or down
// Y it's going to take for RIGHT direction 

USTRUCT(BlueprintType)
struct FRoomStruct
{
	GENERATED_BODY()

	FRoomStruct()
	{
		TileContainer = nullptr;
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

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int Direction;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector2D Position;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int SizeX;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int SizeY;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	ARTiles* TileContainer;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<ARDoor*> Doors;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	ARoom* RoomActor;
	
	int Size() const
	{
		return SizeX*SizeY;
	}
};

struct FMidpoints
{
	FVector Top;
	FVector Bottom;
	FVector Left;
	FVector Right;
	FVector Center;
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

	void ConnectDoors(FRoomStruct& BaseRoom,FRoomStruct& NewRoom);

	void SpawnDoor(FRoomStruct& BaseRoom,FRoomStruct& NewRoom,FVector& DoorAPosition,FVector& DoorBPosition,FRotator& RotRoomA, FRotator& RotRoomB);
	bool DoesCollide(const FRoomStruct& RoomA, const FRoomStruct& RoomB) const;

	void RandomSize();
	
	FRoomStruct& GetRandomRoom();

	FMidpoints GetMidPoints(const FRoomStruct Room,const int Ts = 100);
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

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<ARDoor> DoorClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UStaticMesh* WallMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int RoomsQuantity = 10;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
