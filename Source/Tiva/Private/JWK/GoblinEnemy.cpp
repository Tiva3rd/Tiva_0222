// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/GoblinEnemy.h"

#include "JWK/GoblinFSM.h"

// Sets default values
AGoblinEnemy::AGoblinEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	goblinFSM = CreateDefaultSubobject<UGoblinFSM>( TEXT( "goblinFSM" ) );
}

// Called when the game starts or when spawned
void AGoblinEnemy::BeginPlay()
{
	Super::BeginPlay();
	bIsDie = false;
}

// Called every frame
void AGoblinEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGoblinEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGoblinEnemy::BossTakeDamaged(int32 damage)
{
	BossHP -= damage;

	if (BossHP <= 0)
	{
		BossHP = 0;

		bIsDie = true;
	}
}

