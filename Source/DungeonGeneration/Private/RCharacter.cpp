// Fill out your copyright notice in the Description page of Project Settings.

#include "RCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
// Sets default values
ARCharacter::ARCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));

	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->SetupAttachment(RootComponent);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	
}

// Called when the game starts or when spawned
void ARCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARCharacter::MoveForward(float Axis)
{

	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(),Axis);
	if(Axis>0)
	{
		FRotator NewRot = FRotator(0.0f,-90.0f,0.0f);
		GetMesh()->SetWorldRotation(NewRot);
		
	}
	else if(Axis<0)
	{
		FRotator NewRot = FRotator(0.0f,90.0f,0.0f);
		GetMesh()->SetWorldRotation(NewRot);
	}
}

void ARCharacter::MoveRight(float Axis)
{
	
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector,Axis);
	if(Axis>0)
	{
		FRotator NewRot = FRotator(0.0f,0.0f,0.0f);
		GetMesh()->SetWorldRotation(NewRot);
	
	}
	else if(Axis<0)
	{
		FRotator NewRot = FRotator(0.0f,-180.0f,0.0f);
		GetMesh()->SetWorldRotation(NewRot);
	}
}

// Called every frame
void ARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward",this,&ARCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right",this,&ARCharacter::MoveRight);
}

