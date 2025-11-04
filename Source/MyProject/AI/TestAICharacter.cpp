// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AI/TestAICharacter.h"

// Sets default values
ATestAICharacter::ATestAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

