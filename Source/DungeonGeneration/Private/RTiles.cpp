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

UInstancedStaticMeshComponent* ARTiles::GetInstancedStaticMeshComponent() const
{
	return InstancedStaticMeshComponent;
}

// Called when the game starts or when spawned
void ARTiles::BeginPlay()
{
	Super::BeginPlay();
}

void ARTiles::ChangeTile(UStaticMesh* StaticMesh,int32 InstancedIndex)
{
	// StaticMeshComponent->SetStaticMesh(StaticMesh);
	if(InstancedStaticMeshComponent && StaticMesh)
	{
		// InstancedStaticMeshComponent->SetStaticMesh(StaticMesh);
		UInstancedStaticMeshComponent* NewInstanceComponent = NewObject<UInstancedStaticMeshComponent>(this);
		NewInstanceComponent->RegisterComponentWithWorld(GetWorld());
		NewInstanceComponent->SetStaticMesh(StaticMesh);
		NewInstanceComponent->SetMobility(EComponentMobility::Movable);


		FTransform InstanceTransform;
		InstancedStaticMeshComponent->GetInstanceTransform(InstancedIndex,InstanceTransform);

		int32 NewInstanceIndex = NewInstanceComponent->AddInstance(InstanceTransform);

		// InstancedStaticMeshComponent->UpdateInstanceTransform(InstancedIndex,FTransform());
		// InstancedStaticMeshComponent->RemoveInstance(InstancedIndex);

		NewInstanceComponent->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ARTiles::AddTileInstance(const FVector Location)
{
	if(InstancedStaticMeshComponent)
	{
		for (int32 MaterialIndex = 0; MaterialIndex < InstancedStaticMeshComponent->GetStaticMesh()->GetStaticMaterials().Num(); ++MaterialIndex)
		{
			UMaterialInterface* Material = InstancedStaticMeshComponent->GetStaticMesh()->GetMaterial(MaterialIndex);
			InstancedStaticMeshComponent->SetMaterial(MaterialIndex, Material);
		}
		const int32 InstanceIndex = InstancedStaticMeshComponent->AddInstance(FTransform(Location));
		InstanceIndices.Add(InstanceIndex);
	}
}

// Called every frame
void ARTiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

