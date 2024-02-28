// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossFSM.h"

#include "AIController.h"
#include "AITypes.h"
#include "NavigationSystem.h"
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
	ai = Cast<AAIController>(me->GetController());
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
	FVector destinationToHome = mainTarget->GetActorLocation();
	//// Home 과 Enemy_Boss 의 거리
	//FVector dirToHome = mainTarget->GetActorLocation() - me->GetActorLocation();
	//dirToHome.Normalize();
	float distHome = mainTarget->GetDistanceTo(me);

	FVector destinationToPlayer = playerTarget->GetActorLocation();
	//// Player와 Enemy_Boss 의 거리
	//FVector dirToPlayer = playerTarget->GetActorLocation() - me->GetActorLocation();
	//dirToPlayer.Normalize();
	float distPlayer = playerTarget->GetDistanceTo(me);

	// 만약 EnemyToPlayer 거리가 플레이어 공격 가능 범위보다 멀다면
	if (distPlayer > attackDistPlayer)
	{
		auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		FAIMoveRequest req;
		req.SetAcceptanceRadius(300);
		req.SetGoalLocation(destinationToHome);
		FPathFindingQuery query;
		ai->BuildPathfindingQuery(req, query);
		FPathFindingResult r = ns->FindPathSync(query);
		if (r.Result == ENavigationQueryResult::Success)
		{
			// (추적) AI가 목적지를 향해 이동하게 하고싶다.
			ai->MoveToLocation(destinationToHome, 100);
		}
		/*me->AddMovementInput(dirToHome);
		me->SetActorRotation(dirToHome.ToOrientationRotator());*/


		if (distHome < attackDistHome)
		{
			state = EBoss_Enemy::ATTACKHOME;
		}
	
	}

	// 만약 EnemyToPlayer 거리가 플레이어 공격 가능 범위보다 가깝다면
	else if(distPlayer <= attackDistPlayer)
	{
		ai->MoveToLocation(destinationToPlayer, 100);
		/*me->AddMovementInput(destinationToPlayer);
		me->SetActorRotation(destinationToPlayer.ToOrientationRotator());*/
		if (distHome < attackDistHome)
		{
			state = EBoss_Enemy::ATTACKPLAYER;
		}

	}
	//auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	//FAIMoveRequest req;
	//req.SetAcceptanceRadius(50);
	//req.SetGoalLocation(destination);
	//FPathFindingQuery query;
	//ai->BuildPathfindingQuery(req, query);
	//FPathFindingResult r = ns->FindPathSync(query);
	//// 만약 목적지가 길 위라면
	//if (r.Result == ENavigationQueryResult::Success)
	//{
	//	// (추적) AI가 목적지를 향해 이동하게 하고싶다.
	//	ai->MoveToLocation(destination, 50);
	//}
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

