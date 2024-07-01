// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomStruct.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"



UCLASS()
class DUNGEONGENERATION_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom();

	void CreateFloor();

	void CreateWalls();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<ARTiles> TileClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UStaticMesh* WallMesh;
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FRoomStruct Room;

private:
	void AddTileInstance(FVector Location);
	void ChangeTile(int32 InstanceIndex);
};
