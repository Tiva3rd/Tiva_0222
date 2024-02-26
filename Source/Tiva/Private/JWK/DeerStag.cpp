// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/DeerStag.h"

// Sets default values
ADeerStag::ADeerStag()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADeerStag::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeerStag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADeerStag::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

