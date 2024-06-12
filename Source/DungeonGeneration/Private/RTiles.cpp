// Fill out your copyright notice in the Description page of Project Settings.


#include "RTiles.h"

#include "Components/InstancedStaticMeshComponent.h"
// Sets default values
ARTiles::ARTiles()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	// RootComponent = StaticMeshComponent;
	RootComponent = InstancedStaticMeshComponent;
	
}

// Called when the game starts or when spawned
void ARTiles::BeginPlay()
{
	Super::BeginPlay();
}

void ARTiles::ChangeTile(UStaticMesh* StaticMesh)
{
	// StaticMeshComponent->SetStaticMesh(StaticMesh);
	if(InstancedStaticMeshComponent)
	{
		InstancedStaticMeshComponent->SetStaticMesh(StaticMesh);
	}
}

void ARTiles::AddTileInstance(const FVector Location)
{
	if(InstancedStaticMeshComponent)
	{
		InstancedStaticMeshComponent->AddInstance(FTransform(Location));
	}
}

// Called every frame
void ARTiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

