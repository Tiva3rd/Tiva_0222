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
	AWolf();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UWolfFSM* wolfFSM;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UCharacterMovementComponent* movementComp;

	UPROPERTY(EditAnywhere)
	class USphereComponent* attackComp;

	//////////////////////////////////////// Damaged ////////////////////////////////////////
	UFUNCTION(BlueprintCallable)
	void WolfTakeDamage(float damage);

	UFUNCTION(Server , Reliable)
	void ServerWolfTakeDamage(float damage);

	UFUNCTION(NetMulticast , Reliable)
	void MultiCastWolfTakeDamage(float newHP);

	UPROPERTY(EditAnywhere)
	int32 WolfMaxHP = 4;

	UPROPERTY(Replicated , EditAnywhere)
	int32 WolfHP = WolfMaxHP;


	UPROPERTY(Replicated , EditAnywhere)
	bool bIsAttacked = false;

	UPROPERTY(Replicated , EditAnywhere)
	bool bIsDie = false;


	//////////////////////////////////////// 플레이어 대미지 처리 ////////////////////////////////////////
	UPROPERTY(EditAnywhere)
	class ATivaCharacter* player;

	////////////////////////////////////////////// NetWork //////////////////////////////////////////////
	void FindPlayer();

	UPROPERTY(Replicated , EditAnywhere)
	class AActor* playerTarget;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
