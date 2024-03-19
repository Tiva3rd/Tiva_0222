// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/WolfFSM.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JWK/Wolf.h"
#include "JWK/WolfAnim.h"
#include "Net/UnrealNetwork.h"
#include "Tiva/TivaCharacter.h"


UWolfFSM::UWolfFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWolfFSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AWolf>(GetOwner());
	ai = Cast<AAIController>(me->GetController());

	UE_LOG(LogTemp , Warning , TEXT( "UWolfFSM::BeginPlay : %s" ) , *GetOwner()->GetActorNameOrLabel());
	wolfAnim = Cast<UWolfAnim>(me->GetMesh()->GetAnimInstance());
}


// Called every frame
void UWolfFSM::TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime , TickType , ThisTickFunction);

	switch (state)
	{
	case EWolf::IDLE: TickIdle();
		break;
	case EWolf::MOVE: TickMove();
		break;
	case EWolf::ATTACK: TickAttack();
		break;
	case EWolf::DEAD: TickDead();
		break;
	}


	if (state != EWolf::DEAD && me->bIsDie == true)
		SetState(EWolf::DEAD);

	FString stateString = UEnum::GetValueAsString<EWolf>(state);
	DrawDebugString(GetWorld() , me->GetActorLocation() , stateString , me , FColor::Yellow , 0 , true , 1.0f);
}


//////////////////////////////////////// IDLE ////////////////////////////////////////
void UWolfFSM::TickIdle()
{
	me->FindPlayer(); /*playerTarget->GetDistanceTo( me );*/
	//playerTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (me->playerTarget)
	{
		float distPlayer = me->playerTarget->GetDistanceTo(me);

		UE_LOG(LogTemp , Warning , TEXT( "State : Idle" ));

		if (distPlayer <= chasePlayerReach)
			SetState(EWolf::MOVE);
	}
}


//////////////////////////////////////// MOVE ////////////////////////////////////////
void UWolfFSM::TickMove()
{
	if (nullptr == me->playerTarget)
	{
		me->FindPlayer();
		return;
	}

	if (nullptr == ai)
		return;

	destinationToPlayer = me->playerTarget->GetActorLocation();
	distanceToPlayer = me->playerTarget->GetDistanceTo(me);

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FAIMoveRequest req;
	req.SetAcceptanceRadius(2000);
	req.SetGoalLocation(destinationToPlayer);
	FPathFindingQuery query;
	ai->BuildPathfindingQuery(req , query);
	FPathFindingResult r = ns->FindPathSync(query);

	if (r.Result == ENavigationQueryResult::Success)
	{
		if (distanceToPlayer <= chasePlayerReach)
		{
			UE_LOG(LogTemp , Warning , TEXT( "State : Move" ));
			ai->MoveToLocation(destinationToPlayer , 100);

			if (distanceToPlayer <= attackDistance)
			{
				SetState(EWolf::ATTACK);
				wolfAnim->bIsAttack = true;
			}
		}
	}

	else if (r.Result == ENavigationQueryResult::Fail)
		return;
}


//////////////////////////////////////// ATTACK ////////////////////////////////////////
void UWolfFSM::TickAttack()
{
	curTime += GetWorld()->GetDeltaSeconds();
	UE_LOG(LogTemp , Warning , TEXT( "State : Attack" ));

	if (curTime > attackDelayTime)
	{
		curTime = 0;
		float distance = me->playerTarget->GetDistanceTo(me);
		if (distance > attackDistance)
			SetState(EWolf::MOVE);
		else
			wolfAnim->bIsAttack = true;
	}
}


void UWolfFSM::DoDamageEnd()
{
}

void UWolfFSM::TickDead()
{
	UE_LOG(LogTemp , Warning , TEXT( "WolfState : Dead" ));
	me->GetCharacterMovement()->SetMovementMode(MOVE_None);
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UWolfFSM::SetState(EWolf next)
{
	ServerSetState(next);
}

void UWolfFSM::ServerSetState_Implementation(EWolf nextState)
{
	MultiSetState(nextState);
}

void UWolfFSM::MultiSetState_Implementation(EWolf nextState)
{
	state = nextState;
	curTime = 0;
}

void UWolfFSM::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWolfFSM , state);
}
