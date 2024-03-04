// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossFSM.h"

#include "AIController.h"
#include "AITypes.h"
#include "NavigationSystem.h"
#include "JWK/BossAnim.h"
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

	me = Cast<ABossEnemy>( GetOwner() );
	ai = Cast<AAIController>( me->GetController() );
}


// Called every frame
void UBossFSM::TickComponent( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime , TickType , ThisTickFunction );

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
	//mainTarget= ;
	playerTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (mainTarget)
	{
		SetState( EBoss_Enemy::MOVE );
	}
}

void UBossFSM::TickMove()
{
	bool chaseHome = bossAnim->bIsChaseHome;      // bossAnim 에서 bIsChaseHome = true
	bool chasePlayer = bossAnim->bIsChasePlayer;

	FVector destinationToHome = mainTarget->GetActorLocation();
	float distanceToHome = mainTarget->GetDistanceTo( me );

	FVector destinationToPlayer = playerTarget->GetActorLocation();
	float distanceToPlayer = playerTarget->GetDistanceTo( me );


	auto ns = UNavigationSystemV1::GetNavigationSystem( GetWorld() );
	FAIMoveRequest req;
	req.SetAcceptanceRadius( 300 );
	req.SetGoalLocation( destinationToHome );
	FPathFindingQuery query;
	ai->BuildPathfindingQuery( req , query );
	FPathFindingResult r = ns->FindPathSync( query );


	// 만약 타겟이 있다면
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation( destinationToHome , 50 );
		if (distanceToHome < attackDistance)
		{
			SetState( EBoss_Enemy::ATTACKHOME );
		}

		if (distanceToPlayer < chasePlayerReach)
		{
			ai->MoveToLocation( destinationToPlayer , 50 );
			if (distanceToPlayer < attackDistance)
			{
				SetState( EBoss_Enemy::ATTACKPLAYER );
			}
		}
	}
}

void UBossFSM::TickAttackHome()
{
	curTime += GetWorld()->GetDeltaSeconds();

	if (curTime > attackDelayTime)
	{
		curTime = 0;
		float distance = mainTarget->GetDistanceTo( me );

		if (distance > attackDistance || distance < chasePlayerReach)
			SetState( EBoss_Enemy::MOVE );
	}
}

void UBossFSM::TickAttackPlayer()
{
	curTime += GetWorld()->GetDeltaSeconds();

	if (curTime > attackDelayTime)
	{
		curTime = 0;
		float distance = playerTarget->GetDistanceTo( me );

		if (distance > attackDistance)
			SetState( EBoss_Enemy::MOVE );
	}
}

void UBossFSM::TickDamage()
{
}

void UBossFSM::TakeDamage( int damage )
{
}

void UBossFSM::TickDead()
{
}

void UBossFSM::DoDamageEnd()
{
}

void UBossFSM::SetState( EBoss_Enemy next )
{
	state = next;
	curTime = 0;
}

