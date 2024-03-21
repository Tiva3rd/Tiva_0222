// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossFSM.h"
#include "AIController.h"
#include "AITypes.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JWK/BossEnemy.h"
#include "JWK/BossAnim.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "OJS/houseTargetColumn.h"

UBossFSM::UBossFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}


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

	me->FindChoosePlayer();

	if (state != EBoss_Enemy::DEAD && me->bIsDie == true)
		SetState( EBoss_Enemy::DEAD );
}


//////////////////////////////////////// IDLE ////////////////////////////////////////
void UBossFSM::TickIdle()
{
	if (mainTarget)
		SetState( EBoss_Enemy::MOVE );
}


//////////////////////////////////////// MOVE ////////////////////////////////////////
void UBossFSM::TickMove()
{
	if (nullptr == ai)
		return;

	bool chaseHome = bossAnim->bIsChaseHome;      // bossAnim 에서 bIsChaseHome = true
	bool chasePlayer = bossAnim->bIsChasePlayer;

	FVector destinationToHome = mainTarget->GetActorLocation();
	float distanceToHome = mainTarget->GetDistanceTo( me );

	FVector destinationToPlayer = me->playerTarget->GetActorLocation();
	float distanceToPlayer = me->playerTarget->GetDistanceTo( me );


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
		ai->MoveToLocation( destinationToHome , 150 );
		if (distanceToHome < attackDistance)
		{
			SetState( EBoss_Enemy::ATTACK );
			bossAnim->bIsAttack = true;
		}

		else if (distanceToPlayer < chasePlayerReach)
		{
			ai->MoveToLocation( destinationToPlayer , 150 );
			if (distanceToPlayer < attackDistance)
			{
				SetState( EBoss_Enemy::ATTACK );
				bossAnim->bIsAttack = true;
			}
		}
	}

	else if (r.Result == ENavigationQueryResult::Fail)
		return;
}


//////////////////////////////////////// ATTACK ////////////////////////////////////////
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



void UBossFSM::DoDamageEnd()
{
	SetState( EBoss_Enemy::MOVE );
	me->GetCharacterMovement()->SetMovementMode( MOVE_Walking );
}


//////////////////////////////////////// DEAD ////////////////////////////////////////
void UBossFSM::TickDead()
{
	UE_LOG(LogTemp , Warning , TEXT( "BossState : Dead" ));
	me->GetCharacterMovement()->SetMovementMode( MOVE_None );
	me->GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

void UBossFSM::SetState( EBoss_Enemy next )
{
	ServerSetState( next );
}

void UBossFSM::ServerSetState_Implementation( EBoss_Enemy nextState )
{
	MultiCastSetStaet( nextState );
}

void UBossFSM::MultiCastSetStaet_Implementation( EBoss_Enemy nextState )
{
	state = nextState;
	curTime = 0;
}

void UBossFSM::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( UBossFSM , state );
}

