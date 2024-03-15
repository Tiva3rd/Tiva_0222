// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WolfFSM.h"
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

	virtual  void PossessedBy(AController* NewController) override;

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
	int32 WolfMaxHP = 4;

	UPROPERTY( Replicated , EditAnywhere )
	int32 WolfHP = WolfMaxHP;


	UPROPERTY( Replicated , EditAnywhere )
	bool bIsAttacked = false;

	UPROPERTY( Replicated , EditAnywhere  )
	bool bIsDie = false;


	//////////////////////////////////////// 플레이어 대미지 처리 ////////////////////////////////////////
	UPROPERTY( EditAnywhere )
	class ATivaCharacter* player;

	////////////////////////////////////////////// NetWork //////////////////////////////////////////////
	void FindPlayer();

	UPROPERTY( Replicated , EditAnywhere )
	class AActor* playerTarget;

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
};
