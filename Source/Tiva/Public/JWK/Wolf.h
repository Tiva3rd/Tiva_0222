﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wolf.generated.h"

UCLASS()
class TIVA_API AWolf : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWolf();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick( float DeltaTime ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class UWolfFSM* wolfFSM;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class UCharacterMovementComponent* movementComp;


	//////////////////////////////////////// Damaged ////////////////////////////////////////
	UFUNCTION( BlueprintCallable )
	void WolfTakeDamage( float damage );

	UPROPERTY( EditAnywhere )
	int32 WolfMaxHP = 3;

	UPROPERTY( EditAnywhere )
	int32 WolfHP = WolfMaxHP;

	UPROPERTY( EditAnywhere )
	bool bIsAttacked = false;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	bool bIsDie = false;


	//////////////////////////////////////// 플레이어 대미지 처리 ////////////////////////////////////////
	UPROPERTY( EditAnywhere )
	class ATivaCharacter* player;
};
