// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/Wolf.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "JWK/WolfFSM.h"

// Sets default values
AWolf::AWolf()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	wolfFSM = CreateDefaultSubobject<UWolfFSM>( TEXT( "wolfFSM" ) );

	movementComp = CreateDefaultSubobject<UCharacterMovementComponent>( TEXT( "movementComp" ) );
}

// Called when the game starts or when spawned
void AWolf::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWolf::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AWolf::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

}

void AWolf::WolfTakeDamage( float damage )
{
	wolfFSM->TakeDamage( damage );
}

