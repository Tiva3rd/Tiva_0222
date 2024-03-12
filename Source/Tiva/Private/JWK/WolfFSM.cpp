// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/WolfFSM.h"

#include "AIController.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JWK/Wolf.h"
#include "JWK/WolfAnim.h"

// Sets default values for this component's properties
UWolfFSM::UWolfFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UWolfFSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AWolf>( GetOwner() );
	ai = Cast<AAIController>( me->GetController() );
	wolfAnim = Cast<UWolfAnim>( me->GetMesh()->GetAnimInstance() );

	playerTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UWolfFSM::TickComponent( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime , TickType , ThisTickFunction );

	switch (state)
	{
	case EWolf::IDLE:					 TickIdle();				 break;
	case EWolf::MOVE:					 TickMove();				 break;
	case EWolf::ATTACK:					 TickAttack();				 break;
	case EWolf::DEAD:					 TickDead();				 break;
	}
}

void UWolfFSM::TickIdle()
{
	if (distanceToPlayer <= chasePlayerReach)
		SetState( EWolf::MOVE );


	if (me->bIsDie == true)
		SetState( EWolf::DEAD );
}

void UWolfFSM::TickMove()
{
	destinationToPlayer = playerTarget->GetActorLocation();
	distanceToPlayer = playerTarget->GetDistanceTo( me );

	auto ns = UNavigationSystemV1::GetNavigationSystem( GetWorld() );
	FAIMoveRequest req;
	req.SetAcceptanceRadius( 2000 );
	req.SetGoalLocation( destinationToPlayer );
	FPathFindingQuery query;
	ai->BuildPathfindingQuery( req , query );
	FPathFindingResult r = ns->FindPathSync( query );

	if (r.Result == ENavigationQueryResult::Success)
	{
		if (distanceToPlayer <= chasePlayerReach)
		{
			ai->MoveToLocation( destinationToPlayer , 100 );

			if (distanceToPlayer <= attackDistance)
			{
				SetState( EWolf::ATTACK );
				wolfAnim->bIsAttack = true;
			}
		}
		else
			SetState( EWolf::IDLE );
	}

	else if (r.Result == ENavigationQueryResult::Fail)
		return;

	if (me->bIsDie == true)
		SetState( EWolf::DEAD );
}

void UWolfFSM::TickAttack()
{
	curTime += GetWorld()->GetDeltaSeconds();

	if (curTime > attackDelayTime)
	{
		curTime = 0;
		float distance = playerTarget->GetDistanceTo( me );
		if (distance > attackDistance)
			SetState( EWolf::MOVE );
		else
			wolfAnim->bIsAttack = true;
	}

	if (me->bIsDie == true)
		SetState( EWolf::DEAD );
}

void UWolfFSM::TakeDamage( int32 damage )
{
	me->WolfHP -= damage;
	if (me->WolfHP <= 0)
	{
		me->bIsDie = true;
		me->wolfFSM = 0;
	}
}

void UWolfFSM::DoDamageEnd()
{
}

void UWolfFSM::TickDead()
{
	me->GetCharacterMovement()->SetMovementMode( MOVE_None );
	me->GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

void UWolfFSM::SetState( EWolf next )
{
	state = next;
	curTime = 0;
}

