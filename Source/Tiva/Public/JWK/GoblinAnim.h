// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoblinFSM.h"
#include "Animation/AnimInstance.h"
#include "GoblinAnim.generated.h"

/**
 *
 */
UCLASS()
class TIVA_API UGoblinAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation( float DeltaSeconds ) override;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	float speed = 0;

	UPROPERTY()
	class UGoblinFSM* goblinFSM;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	EGoblin_Enemy state;

	UPROPERTY( EditAnyWhere , BlueprintReadOnly )
	class AGoblinEnemy* me;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	bool bIsChaseHome = true;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	bool bIsChasePlayer = false;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	bool bIsAttack = false;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	bool bIsFire = false;

	UFUNCTION()
	void AnimNotify_AttackStart();

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class UParticleSystem* fireAttack;

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UPROPERTY()
	class UParticleSystemComponent* fire;

};
