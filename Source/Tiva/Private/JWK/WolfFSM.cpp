// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/WolfFSM.h"

#include "AIController.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JWK/Wolf.h"
#include "JWK/WolfAnim.h"
#include "Net/UnrealNetwork.h"
#include "Tiva/TivaCharacter.h"


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
	me->OnRep_FindPlayer();/*playerTarget->GetDistanceTo( me );*/
	//playerTarget = GetWorld()->GetFirstPlayerController()->GetPawn();

	float distPlayer = me->playerTarget->GetDistanceTo( me );

	UE_LOG(LogTemp,Warning,TEXT("State : Idle"));

	if (distPlayer <= chasePlayerReach)
		SetState( EWolf::MOVE );

	if (me->bIsDie == true)
		SetState( EWolf::DEAD );
}

void UWolfFSM::TickMove()
{
	destinationToPlayer = me->playerTarget->GetActorLocation();
	distanceToPlayer = me->playerTarget->GetDistanceTo( me );

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
			UE_LOG( LogTemp , Warning , TEXT( "State : Move" ) );
			ai->MoveToLocation( destinationToPlayer , 100 );

			if (distanceToPlayer <= attackDistance)
			{
				SetState( EWolf::ATTACK );
				wolfAnim->bIsAttack = true;
			}
		}
	}

	else if (r.Result == ENavigationQueryResult::Fail)
		return;

	if (me->bIsDie == true)
		SetState( EWolf::DEAD );
}

void UWolfFSM::TickAttack()
{
	curTime += GetWorld()->GetDeltaSeconds();
	UE_LOG( LogTemp , Warning , TEXT( "State : Attack" ) );
	if (curTime > attackDelayTime)
	{
		curTime = 0;
		float distance = me->playerTarget->GetDistanceTo( me );
		if (distance > attackDistance)
			SetState( EWolf::MOVE );
		else
			wolfAnim->bIsAttack = true;
	}

	if (me->bIsDie == true)
		SetState( EWolf::DEAD );
}


void UWolfFSM::DoDamageEnd()
{
}

void UWolfFSM::TickDead()
{
	UE_LOG( LogTemp , Warning , TEXT( "State : Dead" ) );
	me->GetCharacterMovement()->SetMovementMode( MOVE_None );
	me->GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

void UWolfFSM::SetState( EWolf next )
{
	if (me->HasAuthority())
	{
		state = next;
	}
	curTime = 0;
}

void UWolfFSM::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( UWolfFSM , state );
}
