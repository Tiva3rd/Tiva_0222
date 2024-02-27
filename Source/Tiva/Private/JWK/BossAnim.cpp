// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossAnim.h"
#include "JWK/BossEnemy.h"

void UBossAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	me = Cast<ABossEnemy>(TryGetPawnOwner());

	if (me != nullptr)
	{
		bossFSM = me->bossFSM;
	}
	else
	{
		return;
	}
}

void UBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(bossFSM == nullptr) return;
}
//
//void UBossAnim::AnimNotify_AttackEnd()
//{
//}
//
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
