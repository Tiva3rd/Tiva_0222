// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossFSM.h"

#include "AIController.h"
#include "AITypes.h"
#include "NavigationSystem.h"
#include "JWK/BossAnim.h"
#include "JWK/BossEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "OJS/houseTargetColumn.h"

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
	bossAnim = Cast<UBossAnim>( me->GetMesh()->GetAnimInstance() );

	TArray<AActor*> TempArray;
	UGameplayStatics::GetAllActorsOfClass( GetWorld() , AhouseTargetColumn::StaticClass() , TempArray );
	for (int32 i = 0; i < TempArray.Num(); ++i)
	{
		mainTarget = Cast<AhouseTargetColumn>( TempArray[i] );

		if (mainTarget->GetActorLabel().Contains( TEXT( "BP_houseTargetColumn" ) ))
		{
			break;
		}
	}

	if (nullptr == mainTarget)
		return;
}


// Called every frame
void UBossFSM::TickComponent( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime , TickType , ThisTickFunction );

	switch (state)
	{
	case EBoss_Enemy::IDLE:				        TickIdle();				break;
	case EBoss_Enemy::MOVE:				        TickMove();				break;
	case EBoss_Enemy::ATTACK:					TickAttack();			break;
	case EBoss_Enemy::DEAD:				        TickDead();				break;
	}
}

void UBossFSM::TickIdle()
{
	playerTarget = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (mainTarget)
		SetState( EBoss_Enemy::MOVE );
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
	req.SetAcceptanceRadius( 2000 );
	req.SetGoalLocation( destinationToHome );
	FPathFindingQuery query;
	ai->BuildPathfindingQuery( req , query );
	FPathFindingResult r = ns->FindPathSync( query );


	// 만약 타겟이 있다면
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation( destinationToHome , 100 );
		if (distanceToHome < attackDistance)
		{
			SetState( EBoss_Enemy::ATTACK );
			bossAnim->bIsAttack = true;
		}

		else if (distanceToPlayer < chasePlayerReach)
		{
			ai->MoveToLocation( destinationToPlayer , 100 );
			if (distanceToPlayer < attackDistance)
			{
				SetState( EBoss_Enemy::ATTACK );
				bossAnim->bIsAttack = true;
			}
		}
	}
}

void UBossFSM::TickAttack()
{
	curTime += GetWorld()->GetDeltaSeconds();

	if (curTime > attackDelayTime)
	{
		curTime = 0;
		float distance = mainTarget->GetDistanceTo( me );

		if (distance > attackDistance/* || distance < chasePlayerReach*/)
			SetState( EBoss_Enemy::MOVE );
		else
			bossAnim->bIsAttack = true;
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

