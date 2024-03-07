// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossFSM.h"
#include "BossAnim.generated.h"

/**
 *
 */
UCLASS()
class TIVA_API UBossAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation( float DeltaSeconds ) override;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	float speed = 0;

	UPROPERTY()
	class UBossFSM* bossFSM;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	EBoss_Enemy state;

	UPROPERTY( EditAnyWhere , BlueprintReadOnly )
	class ABossEnemy* me;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	bool bIsChaseHome = true;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	bool bIsChasePlayer = false;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	bool bIsAttack = false;

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UPROPERTY( EditAnywhere , BlueprintReadOnly )
	class ABossEnemy* bIsDie;
};
