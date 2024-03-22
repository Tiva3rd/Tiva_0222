// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/Wolf.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "JWK/WolfFSM.h"
#include "EngineUtils.h"
#include "JWK/WolfAnim.h"
#include "Net/UnrealNetwork.h"
#include "Tiva/TivaCharacter.h"


AWolf::AWolf()
{
	PrimaryActorTick.bCanEverTick = true;

	wolfFSM = CreateDefaultSubobject<UWolfFSM>(TEXT("wolfFSM"));

	movementComp = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("movementComp"));

	attackComp = CreateDefaultSubobject<USphereComponent>(TEXT("attackSphere"));
	attackComp->SetupAttachment(GetMesh() , TEXT("BiteAttack"));
	attackComp->SetWorldScale3D(FVector(1.0f));

	bReplicates = true;
	SetReplicateMovement(true);
}

void AWolf::BeginPlay()
{
	Super::BeginPlay();

	attackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	NetUpdateFrequency = 100;
}

void AWolf::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp , Warning , TEXT( "AWolf::PossessedBy" ));
}

void AWolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


//////////////////////////////////////// 공격 당함 ////////////////////////////////////////
void AWolf::WolfTakeDamage(float damage)
{
	ServerWolfTakeDamage(damage);
}

void AWolf::ServerWolfTakeDamage_Implementation(float damage)
{
	WolfHP -= damage;
	MultiCastWolfTakeDamage(WolfHP);
}

void AWolf::MultiCastWolfTakeDamage_Implementation(float newHP)
{
	WolfHP = newHP;

	if (WolfHP <= 0)
	{
		WolfHP = 0;

		bIsDie = true;

		auto anim = Cast<UWolfAnim>(GetMesh()->GetAnimInstance());
		anim->PlayDeathAnimation();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void AWolf::FindPlayer()
{
	if (playerTarget)
		return;
	// 레벨에 있는 ATivaCharacter 객체들을 다 검사해서 chasePlayerReach안에 있고
	// 그중에서도 가장 까가운녀석을 내 playerTarget으로 지정
	auto pc = GetWorld()->GetFirstPlayerController();
	if (pc->HasAuthority())
	{
		playerTarget = nullptr;
		float tempDist = wolfFSM->chasePlayerReach;


		for (TActorIterator<ATivaCharacter> IT(GetWorld()); IT; ++IT)
		{
			ATivaCharacter* newPlayerCharacter = *IT;

			// 플레이어와 나의 거리를 측정해서
			float temp = newPlayerCharacter->GetDistanceTo(this);

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


void AWolf::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWolf , WolfHP);
	DOREPLIFETIME(AWolf , bIsDie);
	DOREPLIFETIME(AWolf , playerTarget);
}
