// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RDungeonManager.h"
#include "GameFramework/Actor.h"
#include "RDoor.generated.h"

class UArrowComponent;
class UBoxComponent;

UCLASS()
class DUNGEONGENERATION_API ARDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Door")
	FRoomStruct NextRoom;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Door")
	ARDoor* ConnectedDoor;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* GetStaticMesh() const;
	
	void ConnectNextRoom(FRoomStruct& Room);
	void ConnectNextDoor(ARDoor* Door);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Door")
	int TileSize = 60;
};
