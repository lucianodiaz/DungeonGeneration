// Fill out your copyright notice in the Description page of Project Settings.


#include "RDungeonManager.h"

#include "RDoor.h"
#include "Room.h"
#include "RTiles.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
					UE_LOG(LogTemp,Log,TEXT("Collide NewRoom"));
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
				ConnectDoors(BaseRoom,NewRoom);
				AddRoomToList(NewRoom);

				NewRoom.RoomActor->SetActorHiddenInGame(true);
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
		FVector(BaseX, BaseY - (NewRoom.SizeY * TileSize), 0.0f), // LEFT (0)
		FVector(BaseX + (BaseRoom.SizeX * TileSize), BaseY, 0.0f), // UP (1)
		FVector(BaseX, BaseY + (BaseRoom.SizeY * TileSize), 0.0f), // RIGHT (2)
		FVector(BaseX - (NewRoom.SizeX * TileSize), BaseY, 0.0f) // DOWN (3)
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



	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARTiles* TileContainer = GetWorld()->SpawnActor<ARTiles>(TileClass,FVector::ZeroVector,FRotator::ZeroRotator,SpawnParameters);
	
	TileContainer->AttachToActor(FirstNode,FAttachmentTransformRules::KeepRelativeTransform);
	for(int i=0; i<Room.Size(); i++)
	{
		const int Row = i / Room.SizeY;
		const int Col = i %  Room.SizeY;

		const float X = (Row * TileSize) + Room.Position.X;
		const float Y = (Col * TileSize) + Room.Position.Y;
		FVector Location = FVector(X,Y,0.0f);
		
		
		TileContainer->AddTileInstance(Location);
		
	}
	Room.TileContainer = TileContainer;
	Room.RoomActor = FirstNode;
	FirstNode->Room = Room;
	
}

void ARDungeonManager::CreateWalls(FRoomStruct& Room)
{
	
	for (int i=0; i<Room.TileContainer->InstanceIndices.Num();i++)
	{
		

		const int Row = i / Room.SizeY;
		const int Col = i % Room.SizeY;

		
		if((Row == 0 || Row == Room.SizeX-1)|| (Col == 0 || Col == SizeY-1))
		{
			//Do change Tile
			Room.TileContainer->ChangeTile(WallMesh,i);
		}
		
		
	}
}

void ARDungeonManager::ConnectDoors(FRoomStruct& BaseRoom, FRoomStruct& NewRoom)
{
	FMidpoints MidNewRoom = GetMidPoints(NewRoom);
	FMidpoints MidBaseRoom = GetMidPoints(BaseRoom);


	FRotator RightDirection = FRotator(0.0f,180.0f,0.0f);
	FRotator ForwardDirection = FRotator(0.0f,90.0f,0.0f);
	FRotator LeftDirection = FRotator(0.0f,0.0f,0.0f);
	FRotator BackDirection = FRotator(0.0f,270.0f,0.0f);
	switch (NewRoom.Direction)
	{
	case 0: //left
		//Connect Rigth side of new room with Left side Base Room
			SpawnDoor(BaseRoom, NewRoom, MidNewRoom.Right,
					  MidBaseRoom.Left,RightDirection,LeftDirection);
			break;

	case 1: //up
		//Connect Bottom side of new room with TOP side of Base room
			SpawnDoor(BaseRoom, NewRoom, MidNewRoom.Bottom,
					  MidBaseRoom.Top,BackDirection,ForwardDirection);
			break;
	case 2: //right
		//Connect Left side of New Room with Right Side of base Room
			SpawnDoor(BaseRoom, NewRoom, MidNewRoom.Left,
					  MidBaseRoom.Right,LeftDirection,RightDirection);
			break;

	
	case 3: //down
			//connect Top side of New room with Bottom side of base room

			SpawnDoor(BaseRoom, NewRoom, MidNewRoom.Top,
					  MidBaseRoom.Bottom,ForwardDirection,BackDirection);
			break;

		
	default:
		break;
	}
	
				
}

void ARDungeonManager::SpawnDoor(FRoomStruct& BaseRoom, FRoomStruct& NewRoom, FVector& DoorAPosition,
	FVector& DoorBPosition,FRotator& RotRoomA, FRotator& RotRoomB)
{
	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	auto NewRoomDoor = GetWorld()->SpawnActor<ARDoor>(DoorClass,DoorAPosition,RotRoomA,SpawnParameters);
	auto BaseRoomDoor = GetWorld()->SpawnActor<ARDoor>(DoorClass,DoorBPosition,RotRoomB,SpawnParameters);
	
	NewRoomDoor->ConnectNextDoor(BaseRoomDoor);
	NewRoomDoor->ConnectNextRoom(BaseRoom);

	BaseRoomDoor->ConnectNextDoor(NewRoomDoor);
	BaseRoomDoor->ConnectNextRoom(NewRoom);

	NewRoomDoor->AttachToActor(NewRoom.TileContainer,FAttachmentTransformRules::KeepRelativeTransform);
	BaseRoomDoor->AttachToActor(BaseRoom.TileContainer,FAttachmentTransformRules::KeepRelativeTransform);
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
	SizeX = FMath::RandRange(8,16);
	SizeY = FMath::RandRange(8,16);
}

FRoomStruct& ARDungeonManager::GetRandomRoom()
{
	const int Index =  FMath::RandRange(0,Rooms.Num()-1);

	return Rooms[Index];
}

FMidpoints ARDungeonManager::GetMidPoints(const FRoomStruct Room,const int Ts)
{
	FMidpoints Midpoints;


	Midpoints.Left = FVector(Room.Position.X + (((Room.SizeX-1) * Ts) / 2), Room.Position.Y+(Ts/1.8),0.0f);
	Midpoints.Bottom = FVector(Room.Position.X + (Ts/1.8), Room.Position.Y + (((Room.SizeY-1.5) * Ts) / 2),0.0f);

	Midpoints.Right = FVector(Room.Position.X + (((Room.SizeX-1) * Ts) / 2), Room.Position.Y + ((Room.SizeY-1.5) * Ts),0.0f);

	Midpoints.Top = FVector(Room.Position.X + ((Room.SizeX-1.5) * Ts), Room.Position.Y + (((Room.SizeY-1) * Ts) / 2),0.0f);

	Midpoints.Center = FVector(Room.Position.X + ((Room.SizeX)* Ts)/2,Room.Position.Y + ((Room.SizeY) * Ts)/2,0.0f);
	
	return Midpoints;
	
}

// Called every frame
void ARDungeonManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

