// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossAnim.h"
#include "Components/SphereComponent.h"
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

	if (nullptr == bossFSM)
	{
		UE_LOG(LogTemp , Warning , TEXT("nullptr == bossFSM"));
		return;
	}

	state = bossFSM->state;
}

void UBossAnim::AnimNotify_BossAttack()
{
	// 공격 시작하면 Collision On
	me->attackSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UBossAnim::AnimNotify_BossAttackCollision()
{
	// 공격 끝나면 Collision OFF
	me->attackSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
