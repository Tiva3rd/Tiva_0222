// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JWK/WolfFSM.h"
#include "WolfAnim.generated.h"

/**
 *
 */
UCLASS()
class TIVA_API UWolfAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation( float DeltaSeconds ) override;

	UPROPERTY( EditAnywhere , BlueprintReadOnly )
	float speed = 1000;

	UPROPERTY()
	class UWolfFSM* wolfFSM;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	EWolf state;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class AWolf* me;



	UPROPERTY( EditAnywhere , BlueprintReadOnly )
	bool bIsChasePlayer = false;

	UPROPERTY( EditAnywhere , BlueprintReadOnly )
	bool bIsAttack = false;

	UFUNCTION()
	void AnimNotify_WolfAttackEnd();

	UPROPERTY( EditAnywhere )
	class UAnimMontage* WolfDeath;

	void PlayDeathAnimation();
	//UPROPERTY( EditAnywhere , BlueprintReadOnly )
	//class AWolf* bIsDie;
};
