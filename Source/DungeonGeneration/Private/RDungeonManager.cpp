// Fill out your copyright notice in the Description page of Project Settings.


#include "RDungeonManager.h"

#include "Room.h"
#include "RTiles.h"

// Sets default values
ARDungeonManager::ARDungeonManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileSize = 100;
	SizeX = 7;
	SizeY = 7;
}

// Called when the game starts or when spawned
void ARDungeonManager::BeginPlay()
{
	Super::BeginPlay();

	CreateFirstRoom();
	CreateRooms();
}

void ARDungeonManager::CreateFirstRoom()
{
	FRoomStruct NewRoom;

	NewRoom.Position = FVector2D(0,0);
	NewRoom.SizeX = SizeX;
	NewRoom.SizeY = SizeY;
	NewRoom.ID = 0;

	CreateFloor(NewRoom);
	CreateWalls(NewRoom);
	AddRoomToList(NewRoom);
}

void ARDungeonManager::CreateRooms()
{
	for(int i=1;i<RoomsQuantity;i++)
	{
		bool bFoundPosition = false;

		
		while(!bFoundPosition)
		{
			bool bCollision= false;
			
			FRoomStruct NewRoom;

			auto& BaseRoom = GetRandomRoom();
			GenerateRoomAdjacent(BaseRoom,NewRoom);

			for(auto&& Room : Rooms)
			{
				if(DoesCollide(NewRoom,Room))
				{
					bCollision = true;
					break;
				}
			}
			if(!bCollision)
			{
				bFoundPosition = true;
				NewRoom.CreatedByID = BaseRoom.ID;
				NewRoom.ID = i;
				CreateFloor(NewRoom);
				CreateWalls(NewRoom);
				AddRoomToList(NewRoom);
			}
		}
	}
}

void ARDungeonManager::GenerateRoomAdjacent(const FRoomStruct& BaseRoom, FRoomStruct& NewRoom)
{
	const float BaseX = BaseRoom.Position.X;
	const float BaseY = BaseRoom.Position.Y;

	RandomSize();
	NewRoom.SizeX = SizeX;
	NewRoom.SizeY = SizeY;
	//Position MUST be calculated by position of room BASE bc if We only use BASEX or BASEY
	//NewRoom maybe spawn far from base room, we don't want that
	TArray<FVector> NewPositions =
		{
			FVector((BaseX + BaseRoom.SizeX * TileSize),BaseY,0.0f),//Right
		
			FVector((BaseX - BaseRoom.SizeX + TileSize),BaseY,0.0f),//Left THIS
		
			FVector(BaseX,BaseY + (BaseRoom.SizeY * TileSize),0.0f),//Up 
		
			FVector(BaseX,BaseY - (BaseRoom.SizeY + TileSize),0.0f)//Down 
		};
	const int Index =  FMath::RandRange(0,NewPositions.Num()-1);

	NewRoom.Position = FVector2D(NewPositions[Index].X,NewPositions[Index].Y);
	NewRoom.Direction = Index;
}

void ARDungeonManager::AddRoomToList(const FRoomStruct& Room)
{
	Rooms.Push(Room);
}

void ARDungeonManager::CreateFloor(FRoomStruct& Room)
{

	ensureMsgf(TileClass,TEXT("You Need setup the Tile for spawning"));
	FActorSpawnParameters SpawnParameter;

	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ARoom* FirstNode = GetWorld()->SpawnActor<ARoom>(RoomClass,FVector::ZeroVector,FRotator::ZeroRotator,SpawnParameter);
	
	FirstNode->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);

	const FString Name = "Room_"+FString::FromInt(Room.ID);
	FirstNode->SetActorLabel(Name);

	
	for(int i=0; i<Room.Size(); i++)
	{
		const int Row = i / Room.SizeY;
		const int Col = i %  Room.SizeY;

		const float X = (Row * TileSize) + Room.Position.X;
		const float Y = (Col * TileSize) + Room.Position.Y;
		FVector Location = FVector(X,Y,0.0f);
		FActorSpawnParameters SpawnParameters;

		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ARTiles* Tile = GetWorld()->SpawnActor<ARTiles>(TileClass,Location,FRotator::ZeroRotator,SpawnParameters);
		
		Tile->AttachToActor(FirstNode,FAttachmentTransformRules::KeepRelativeTransform);
		Room.Tiles.Add(Tile);
	}

	FirstNode->Room = Room;
}

void ARDungeonManager::CreateWalls(FRoomStruct& Room)
{

	for (int i=0; i<Room.Tiles.Num();i++)
	{
		ARTiles* Tiles = Room.Tiles[i];

		const int Row = i / Room.SizeY;
		const int Col = i % Room.SizeY;

		
		if((Row == 0 || Row == Room.SizeX-1)|| (Col == 0 || Col == SizeY-1))
		{
			//Do change Tile
			Tiles->ChangeTile(WallMesh);
		}
		
		
	}
}

bool ARDungeonManager::DoesCollide(const FRoomStruct& RoomA, const FRoomStruct& RoomB) const
{

	if(RoomA.Position.X < RoomB.Position.X + (RoomB.SizeX* TileSize) &&
	   RoomA.Position.X + (RoomA.SizeX * TileSize) > RoomB.Position.X &&
		RoomA.Position.Y < RoomB.Position.Y + (RoomB.SizeY * TileSize) &&
		RoomA.Position.Y + (RoomA.SizeY * TileSize) > RoomB.Position.Y)
	{
		return true;
	}
	return false;
}

void ARDungeonManager::RandomSize()
{
	SizeX = FMath::RandRange(7,15);
	SizeY = FMath::RandRange(7,15);
}

FRoomStruct& ARDungeonManager::GetRandomRoom()
{
	const int Index =  FMath::RandRange(0,Rooms.Num()-1);

	return Rooms[Index];
}

// Called every frame
void ARDungeonManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

