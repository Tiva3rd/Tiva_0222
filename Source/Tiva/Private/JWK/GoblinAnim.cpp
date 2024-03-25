// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/GoblinAnim.h"

#include "Components/SphereComponent.h"
#include "JWK/GoblinEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void UGoblinAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	me = Cast<AGoblinEnemy>(TryGetPawnOwner());

	if (nullptr != me)
		goblinFSM = me->goblinFSM;

	else
		return;
}

void UGoblinAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	me = Cast<AGoblinEnemy>(TryGetPawnOwner());

	if (me)
		goblinFSM = me->goblinFSM;

	if (nullptr == goblinFSM)
		return;

	state = goblinFSM->state;
	bIsAttack = goblinFSM->bIsAttack;
}

void UGoblinAnim::AnimNotify_AttackStart()
{
	FTransform s = this->GetSkelMeshComponent()->GetSocketTransform(TEXT("FireAttack"));

	fire = UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , fireAttack , s);

	me->attackSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle , [&]()
	{
		fire->DestroyComponent();
	} , 2.5 , false);
}

void UGoblinAnim::AnimNotify_AttackEnd()
{
	goblinFSM->bIsAttack = false;
	// UE_LOG(LogTemp , Warning , TEXT("Goblin Attack? : %d") , bIsAttack);
	me->attackSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UGoblinAnim::PlayDeathAnimation()
{
	Montage_Play(GoblinDeath);
}
