// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/Wolf.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "JWK/WolfFSM.h"
#include "EngineUtils.h"
#include "JWK/WolfAnim.h"
#include "Net/UnrealNetwork.h"
#include "Tiva/TivaCharacter.h"


// Sets default values
AWolf::AWolf()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	wolfFSM = CreateDefaultSubobject<UWolfFSM>( TEXT( "wolfFSM" ) );

	movementComp = CreateDefaultSubobject<UCharacterMovementComponent>( TEXT( "movementComp" ) );
	bReplicates = true;
	SetReplicateMovement( true );
}

// Called when the game starts or when spawned
void AWolf::BeginPlay()
{
	Super::BeginPlay();

	NetUpdateFrequency = 100;
}

// Called every frame
void AWolf::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	OnRep_FindPlayer();
}

// Called to bind functionality to input
void AWolf::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

}

void AWolf::WolfTakeDamage( float damage )
{
	WolfHP -= damage;
	if (WolfHP <= 0)
	{
		WolfHP = 0;

		bIsDie = true;

		auto anim = Cast<UWolfAnim>(GetMesh()->GetAnimInstance());
		anim->PlayDeathAnimation();
		//GetCharacterMovement()->DisableMovement();
	}
}


void AWolf::OnRep_FindPlayer()
{
	// 레벨에 있는 ATivaCharacter 객체들을 다 검사해서 chasePlayerReach안에 있고
	// 그중에서도 가장 까가운녀석을 내 playerTarget으로 지정
	if (HasAuthority())
	{
		playerTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
		float tempDist = wolfFSM->chasePlayerReach;


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

			//if (playerTarget != newPlayerCharacter)
			//	playerTarget = newPlayerCharacter;
		}
	}
}


void AWolf::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AWolf , WolfHP );
	DOREPLIFETIME( AWolf , bIsDie );
	DOREPLIFETIME( AWolf , playerTarget );
}

