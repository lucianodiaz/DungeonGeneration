// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTiles.generated.h"

// class UStaticMeshComponent;


UCLASS()
class DUNGEONGENERATION_API ARTiles : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTiles();
	void ChangeTile(UStaticMesh* StaticMesh,int32 InstancedIndex);

	void AddTileInstance(const FVector Location);
	TArray<int32> InstanceIndices;

	UInstancedStaticMeshComponent* GetInstancedStaticMeshComponent() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
