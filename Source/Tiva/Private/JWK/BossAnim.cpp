// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossAnim.h"
#include "JWK/BossEnemy.h"

void UBossAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	me = Cast<ABossEnemy>(TryGetPawnOwner());

	if (nullptr != me)
		bossFSM = me->bossFSM;

	else
		return;
}

void UBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	me = Cast<ABossEnemy>(TryGetPawnOwner());
	if (me)
		bossFSM = me->bossFSM;
	
	if(nullptr == bossFSM)
	{
		UE_LOG(LogTemp , Warning , TEXT("nullptr == bossFSM"));
		return;
	}

	state = bossFSM->state;
}

void UBossAnim::AnimNotify_BossAttack()
{
	//Dealdamage->DealDamage();
}

void UBossAnim::AnimNotify_AttackEnd()
{
	bIsAttack = false;
}

void UBossAnim::AnimNotify_BossDamageEnd()
{
	bossFSM->DoDamageEnd();
}

void UBossAnim::PlayDeathAnimation()
{
	Montage_Play(BossDeath);
}

void UBossAnim::PlayHitAnimation()
{
	Montage_Play(BossHit);
}

//void UBossAnim::AnimNotify_DamageEnd()
//{
//}
//
//void UBossAnim::AnimNotify_DieEnd()
//{
//}
//
//void UBossAnim::AnimNotify_Hit()
//{
//}
