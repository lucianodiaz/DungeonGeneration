// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RDungeonManager.generated.h"

struct FRoomStruct;
class ARoom;
class ARTiles;
class ARDoor;

//For this project we will take this Way
// X it's going to take for Forward or maybe UP direction we don't use Z for UP or down
// Y it's going to take for RIGHT direction 



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

	void GenerateRoomAdjacent(const FRoomStruct& BaseRoom, FRoomStruct& NewRoom,TSubclassOf<ARoom>& RoomClass);
	
	void AddRoomToList(const FRoomStruct& Room);

	void CreateFloor(FRoomStruct& Room);

	void CreateWalls(FRoomStruct& Room);

	void ConnectDoors(FRoomStruct& BaseRoom,FRoomStruct& NewRoom);

	void SpawnDoor(FRoomStruct& BaseRoom,FRoomStruct& NewRoom,FVector& DoorAPosition,FVector& DoorBPosition,FRotator& RotRoomA, FRotator& RotRoomB);
	bool DoesCollide(const FRoomStruct& RoomA, const FRoomStruct& RoomB) const;

	void RandomSize();

	bool ExistTreasureRoom() const;

	bool ExistBoosRoom() const;

	bool ExistTrapRoom() const;
	
	FRoomStruct& GetRandomRoom();

	FMidpoints GetMidPoints(const FRoomStruct Room,const int Ts = 100);
	///VARIABLES
	UPROPERTY(BlueprintReadOnly,meta=(ClampMin=5))
	int SizeX;
	
	UPROPERTY(BlueprintReadOnly,meta=(ClampMin=5))
	int SizeY;

	UPROPERTY(BlueprintReadOnly)
	int TileSize;

	UPROPERTY(BlueprintReadWrite)
	TArray<FRoomStruct> Rooms;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<ARDoor> DoorClass;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<ARoom> NormalRoomClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<ARoom> TreasureRoomClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<ARoom> BossRoomClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<ARoom> TrapRoomClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Creator")
	int RoomsQuantity = 10;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Creator")
	int MaxSize = 18;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Creator")
	int MinSize = 8;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
