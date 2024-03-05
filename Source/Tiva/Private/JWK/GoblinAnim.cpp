// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/GoblinAnim.h"

#include "JWK/GoblinEnemy.h"
#include "Kismet/GameplayStatics.h"

void UGoblinAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	me = Cast<AGoblinEnemy>( TryGetPawnOwner() );

	if (nullptr != me)
		goblinFSM = me->goblinFSM;

	else
		return;
}

void UGoblinAnim::NativeUpdateAnimation( float DeltaSeconds )
{
	Super::NativeUpdateAnimation( DeltaSeconds );

	if (nullptr == goblinFSM)
		return;

	state = goblinFSM->state;
}

void UGoblinAnim::AnimNotify_AttackStart()
{
	FTransform s = this->GetSkelMeshComponent() ->GetSocketTransform( TEXT( "FireAttack" ) );

	UGameplayStatics::SpawnEmitterAtLocation( GetWorld() , fireAttack , s );
}

void UGoblinAnim::AnimNotify_AttackEnd()
{
	bIsAttack = true;
}
