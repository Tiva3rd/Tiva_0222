// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossFSM.h"
#include "JWK/BossEnemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBossFSM::UBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossFSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<ABossEnemy>(GetOwner());
}


// Called every frame
void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (state)
	{
	case EBoss_Enemy::IDLE:				        TickIdle();				break;
	case EBoss_Enemy::MOVE:				        TickMove();				break;
	case EBoss_Enemy::ATTACKHOME:				TickAttackHome();		break;
	case EBoss_Enemy::ATTACKPLAYER:				TickAttackPlayer();		break;
	case EBoss_Enemy::DEAD:				        TickDead();				break;
	}
}

void UBossFSM::TickIdle()
{
	if (mainTarget)
	{
		state = EBoss_Enemy::MOVE;
	}
}

void UBossFSM::TickMove()
{
	// Home 과 Enemy_Boss 의 거리
	FVector dirToHome = mainTarget->GetActorLocation() - me->GetActorLocation();
	dirToHome.Normalize();
	float distHome = mainTarget->GetDistanceTo(me);


	// Player와 Enemy_Boss 의 거리
	FVector dirToPlayer = playerTarget->GetActorLocation() - me->GetActorLocation();
	dirToPlayer.Normalize();
	float distPlayer = playerTarget->GetDistanceTo(me);

	// 만약 EnemyToPlayer 거리가 플레이어 공격 가능 범위보다 멀다면
	if (distPlayer > attackDistPlayer)
	{
		me->AddMovementInput(dirToHome);
		me->SetActorRotation(dirToHome.ToOrientationRotator());


		if (distHome < attackDistHome)
		{
			state = EBoss_Enemy::ATTACKHOME;
		}

		else
		{
			state = EBoss_Enemy::MOVE;
		}
	}

	// 만약 EnemyToPlayer 거리가 플레이어 공격 가능 범위보다 가깝다면
	else if(distPlayer <= attackDistPlayer)
	{
		me->AddMovementInput(dirToPlayer);
		me->SetActorRotation(dirToPlayer.ToOrientationRotator());
		if (distHome < attackDistHome)
		{
			state = EBoss_Enemy::ATTACKPLAYER;
		}

		else
		{
			state = EBoss_Enemy::MOVE;
		}
	}
}

void UBossFSM::TickAttackHome()
{
	curTime += GetWorld()->GetDeltaSeconds();

	if (curTime > attackWaitTime)
	{
		curTime = 0;
		float distance = mainTarget->GetDistanceTo(me);

		if (distance > attackDistHome)
		{
			SetState(EBoss_Enemy::MOVE);
		}

		else
		{
			// 공격 애니메이션
			UE_LOG(LogTemp, Log, TEXT("Attack"));
		}
	}
}

void UBossFSM::TickAttackPlayer()
{
}

void UBossFSM::TickDamage()
{
}

void UBossFSM::TakeDamage(int damage)
{
}

void UBossFSM::TickDead()
{
}

void UBossFSM::DoDamageEnd()
{
}

void UBossFSM::SetState(EBoss_Enemy next)
{
	state = next;
	curTime = 0;
}

