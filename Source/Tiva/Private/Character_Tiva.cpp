// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Tiva.h"

// Sets default values
ACharacter_Tiva::ACharacter_Tiva()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacter_Tiva::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacter_Tiva::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacter_Tiva::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

