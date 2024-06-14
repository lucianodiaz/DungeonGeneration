// Fill out your copyright notice in the Description page of Project Settings.


#include "RDoor.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ARDoor::ARDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	
	RootComponent = StaticMeshComponent;


	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	BoxCollision->SetupAttachment(RootComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));

	ArrowComponent->SetupAttachment(RootComponent);


	ArrowComponent->SetRelativeRotation(FRotator(0.0f,90.0f,0.0f));
	ArrowComponent->SetRelativeLocation(FVector(0.0f,70.0f,0.0f));

	ArrowComponent->ArrowColor = FColor::Yellow;
	ArrowComponent->ArrowLength = 20.0f;
}

// Called when the game starts or when spawned
void ARDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* ARDoor::GetStaticMesh()const
{
	return StaticMeshComponent;
}

void ARDoor::ConnectNextRoom(FRoomStruct& Room)
{
	NextRoom = Room;
}

void ARDoor::ConnectNextDoor(ARDoor* Door)
{
	if(Door)
	{
		ConnectedDoor = Door;
	}
}

