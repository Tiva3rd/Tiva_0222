// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/WolfAnim.h"

#include "JWK/Wolf.h"

void UWolfAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	me = Cast<AWolf>( TryGetPawnOwner() );

	if (nullptr != me)
		wolfFSM = me->wolfFSM;

	else
		return;
}

void UWolfAnim::NativeUpdateAnimation( float DeltaSeconds )
{
	Super::NativeUpdateAnimation( DeltaSeconds );
	if (nullptr == wolfFSM)
		return;

	state = wolfFSM->state;
}

void UWolfAnim::AnimNotify_WolfAttackEnd()
{
	bIsAttack = false;
}
