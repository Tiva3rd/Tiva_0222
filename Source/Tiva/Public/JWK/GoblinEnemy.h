// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GoblinEnemy.generated.h"

UCLASS()
class TIVA_API AGoblinEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGoblinEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick( float DeltaTime ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	// GoblinFSM 을 컴포넌트로
	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class UGoblinFSM* goblinFSM;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class UCharacterMovementComponent* movementComp;



	//////////////////////////////////////// 공격 당함 ////////////////////////////////////////
	UFUNCTION( BlueprintCallable )
	void GoblinTakeDamaged( int32 damage );

	UPROPERTY( EditAnywhere )
	int32 GoblinMaxHP = 3;

	UPROPERTY( Replicated , EditAnywhere )
	int32 GoblinHP = GoblinMaxHP;

	UPROPERTY( EditAnywhere )
	bool bIsAttacked = false;

	UPROPERTY( Replicated , EditAnywhere )
	bool bIsDie = false;


	//////////////////////////////////////// 플레이어 대미지 처리 ////////////////////////////////////////
	UPROPERTY( EditAnywhere )
	class ATivaCharacter* player;

	////////////////////////////////////////////// NetWork //////////////////////////////////////////////
	UFUNCTION()
	void FindPlayer();

	UPROPERTY( Replicated , EditAnywhere )
	class AActor* playerTarget;

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
};
