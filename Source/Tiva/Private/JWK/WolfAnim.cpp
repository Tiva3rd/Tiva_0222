// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/WolfAnim.h"

#include "Components/SphereComponent.h"
#include "JWK/Wolf.h"
#include "Net/UnrealNetwork.h"

void UWolfAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	me = Cast<AWolf>(TryGetPawnOwner());

	if (nullptr != me)
		wolfFSM = me->wolfFSM;

	else
		return;
}

void UWolfAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	me = Cast<AWolf>(TryGetPawnOwner());

	if (me)
		wolfFSM = me->wolfFSM;

	if (nullptr == wolfFSM)
	{
		UE_LOG(LogTemp , Warning , TEXT("nullptr == wolfFSM"));
		return;
	}

	state = wolfFSM->state;
}

void UWolfAnim::AnimNotify_WolfAttackStart()
{
	me->attackComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UWolfAnim::AnimNotify_WolfAttackEnd()
{
	me->attackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsAttack = false;
}

void UWolfAnim::PlayDeathAnimation()
{
	Montage_Play(WolfDeath);
}
