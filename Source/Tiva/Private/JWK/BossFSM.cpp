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
	FVector dir = mainTarget->GetActorLocation() - me->GetActorLocation();
	dir.Normalize();

	me->AddMovementInput(dir);
	float distance = mainTarget->GetDistanceTo(me);

	if (distance < attackDist)
	{
		state = EBoss_Enemy::ATTACKHOME;
	}
}

void UBossFSM::TickAttackHome()
{
	curTime += GetWorld()->GetDeltaSeconds();

	if(curTime > attackWaitTime)
	{
		curTime=0;
		float distance = mainTarget->GetDistanceTo(me);

		if(distance> attackDist)
		{
			SetState(EBoss_Enemy::MOVE);
		}

		else
		{
			// 공격 애니메이션
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

