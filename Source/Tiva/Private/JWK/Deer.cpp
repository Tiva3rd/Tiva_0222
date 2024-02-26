// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/Deer.h"

// Sets default values
ADeer::ADeer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADeer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADeer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

