// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

#include "RTiles.h"

// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Room.SizeX = 10;
	Room.SizeY = 10;
	Room.RoomType = Normal;
}

void ARoom::CreateFloor()
{
	ensureMsgf(TileClass, TEXT("You Need setup the Tile for spawning"));
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
	ARTiles* TileContainer = GetWorld()->SpawnActor<ARTiles>(TileClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	TileContainer->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

	for (int i = 0; i < Room.Size(); i++)
	{
		const int Row = i / Room.SizeY;
		const int Col = i % Room.SizeY;
		const float X = (Row * 100) + Room.Position.X;
		const float Y = (Col * 100) + Room.Position.Y;
		const FVector Location = FVector(X, Y, 0.0f);
		TileContainer->AddTileInstance(Location);
	}

	Room.TileContainer = TileContainer;
	
}

void ARoom::CreateWalls()
{
	for (int i = 0; i < Room.TileContainer->InstanceIndices.Num(); i++)
	{
		const int Row = i / Room.SizeY;
		const int Col = i % Room.SizeY;

		if((Row == 0 || Row == Room.SizeX-1)|| (Col == 0 || Col == Room.SizeY-1))
		{
			ChangeTile(i);
		}
	}
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoom::AddTileInstance(FVector Location)
{
	if (Room.TileContainer)
	{
		Room.TileContainer->AddTileInstance(Location);
	}
}

void ARoom::ChangeTile(int32 InstanceIndex)
{
	if (Room.TileContainer)
	{
		Room.TileContainer->ChangeTile(WallMesh, InstanceIndex);
	}
}
