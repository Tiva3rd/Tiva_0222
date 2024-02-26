// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossEnemy.h"

// Sets default values
ABossEnemy::ABossEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	swordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("swordComp"));
	swordComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempSword(TEXT("/Script/Engine.StaticMesh'/Game/00_JWK/ABP/Boss/Sword/AatroxSword.AatroxSword'"));

	if(tempSword.Succeeded())
	{
		swordComp->SetStaticMesh(tempSword.Object);
		swordComp->SetRelativeLocationAndRotation(FVector(190, -20, 5), FRotator(90, 0, 90));
		swordComp->SetWorldScale3D(FVector(0.25f));
	}
}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

