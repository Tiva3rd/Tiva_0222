// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/GoblinFSM.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JWK/GoblinAnim.h"
#include "JWK/GoblinEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "OJS/houseTargetColumn.h"

// Sets default values for this component's properties
UGoblinFSM::UGoblinFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGoblinFSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AGoblinEnemy>( GetOwner() );
	ai = Cast<AAIController>( me->GetController() );
	goblinAnim = Cast<UGoblinAnim>( me->GetMesh()->GetAnimInstance() );

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
void UGoblinFSM::TickComponent( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime , TickType , ThisTickFunction );

	switch (state)
	{
	case EGoblin_Enemy::IDLE:				        TickIdle();				break;
	case EGoblin_Enemy::MOVE:				        TickMove();				break;
	case EGoblin_Enemy::ATTACK:						TickAttack();			break;
	case EGoblin_Enemy::DEAD:				        TickDead();				break;
	}

	me->FindChoosePlayer();

	//if (nullptr == me->playerTarget)
	//{
	//	me->FindChoosePlayer();
	//	return;
	//}

	// 죽음상태가 아닐 때 bIsDie 값이 들어오면
	if (state != EGoblin_Enemy::DEAD && me->bIsDie == true)
		SetState( EGoblin_Enemy::DEAD );
}


//////////////////////////////////////// IDLE ////////////////////////////////////////
void UGoblinFSM::TickIdle()
{
	// mainTarget 있으면 State = Move  ==> 일단 무조건 Move
	if (mainTarget)
		SetState( EGoblin_Enemy::MOVE );
}


//////////////////////////////////////// MOVE ////////////////////////////////////////
void UGoblinFSM::TickMove()
{

	// AI Controller 널 값이면 다시 AI Controller 찾아와!!
	if (nullptr == ai)
		return;

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
		ai->MoveToLocation( destinationToHome , 100 );
		if (distanceToHome <= attackDistance)
		{
			SetState( EGoblin_Enemy::ATTACK );
			goblinAnim->bIsAttack = true;
		}

		else if (distanceToPlayer < chasePlayerReach)
		{
			ai->MoveToLocation( destinationToPlayer , 100 );
			if (distanceToPlayer <= attackDistance)
			{
				SetState( EGoblin_Enemy::ATTACK );
				goblinAnim->bIsAttack = true;
			}
		}
	}
	else if (r.Result == ENavigationQueryResult::Fail)
	{
		return;
	}
}


//////////////////////////////////////// ATTACK ////////////////////////////////////////
void UGoblinFSM::TickAttack()
{
	curTime += GetWorld()->GetDeltaSeconds();

	if (curTime > attackDelayTime)
	{
		curTime = 0;
		float distance = mainTarget->GetDistanceTo( me );

		if (distance >= attackDistance)
			SetState( EGoblin_Enemy::MOVE );
		else
			goblinAnim->bIsAttack = true;
	}
}


//////////////////////////////////////// DEAD ////////////////////////////////////////
void UGoblinFSM::TickDead()
{
	me->GetCharacterMovement()->SetMovementMode( MOVE_None );
	me->GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

void UGoblinFSM::SetState( EGoblin_Enemy next )
{
	ServerSetState( next );
}

void UGoblinFSM::ServerSetState_Implementation( EGoblin_Enemy nextState )
{
	MultiCastSetStaet( nextState );
}

void UGoblinFSM::MultiCastSetStaet_Implementation( EGoblin_Enemy nextState )
{
	state = nextState;
	curTime = 0;
}

void UGoblinFSM::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( UGoblinFSM , state );
}


