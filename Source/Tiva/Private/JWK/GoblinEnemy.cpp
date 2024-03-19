// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/GoblinEnemy.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JWK/GoblinFSM.h"
#include "EngineUtils.h"
#include "JWK/GoblinAnim.h"
#include "Net/UnrealNetwork.h"
#include "Tiva/TivaCharacter.h"

// Sets default values
AGoblinEnemy::AGoblinEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	goblinFSM = CreateDefaultSubobject<UGoblinFSM>( TEXT( "goblinFSM" ) );

	movementComp = CreateDefaultSubobject<UCharacterMovementComponent>( TEXT( "movementComp" ) );
	bReplicates = true;
	SetReplicateMovement( true );
}

// Called when the game starts or when spawned
void AGoblinEnemy::BeginPlay()
{
	Super::BeginPlay();

	NetUpdateFrequency = 100;
}

// Called every frame
void AGoblinEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AGoblinEnemy::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

}

void AGoblinEnemy::GoblinTakeDamaged( int32 damage )
{
	GoblinHP -= damage;

	if (GoblinHP <= 0)
	{
		GoblinHP = 0;

		bIsDie = true;

		auto anim = Cast<UGoblinAnim>( GetMesh()->GetAnimInstance() );
		anim->PlayDeathAnimation();
		GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	}
}

void AGoblinEnemy::FindChoosePlayer()
{
	// GoblinFSM 에서 TickIdle에서 PlayerSearch를 호출
	//if (playerTarget)
	//	return;

	// 레벨에 있는 ATivaCharacter 객체들을 다 검사해서 chasePlayerReach안에 있고
	// 그중에서도 가장 까가운녀석을 내 playerTarget으로 지정
	auto pc = GetWorld()->GetFirstPlayerController();
	if (pc->HasAuthority())
	{
		playerTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
		float tempDist = goblinFSM->chasePlayerReach;


		for (TActorIterator<ATivaCharacter> IT( GetWorld() ); IT; ++IT)
		{
			ATivaCharacter* newPlayerCharacter = *IT;

			// 플레이어와 나의 거리를 측정해서
			float temp = newPlayerCharacter->GetDistanceTo( this );

			// 만약 temp가 tempDist보다 가깝다면
			if (temp <= tempDist)
			{
				// NewPlayer로 기억하고싶다.
				playerTarget = newPlayerCharacter;

				// tempDist = temp;
				tempDist = temp;
			}
		}
	}
}

void AGoblinEnemy::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AGoblinEnemy , GoblinHP );
	DOREPLIFETIME( AGoblinEnemy , bIsDie );
	DOREPLIFETIME( AGoblinEnemy , playerTarget );
}

