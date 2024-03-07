// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossEnemy.generated.h"

UCLASS()
class TIVA_API ABossEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//////////////////////////////////////// 컴포넌트 ////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* swordComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBossFSM* bossFSM;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class UCharacterMovementComponent* movementComp;


	//////////////////////////////////////// Damaged ////////////////////////////////////////
	UFUNCTION( BlueprintCallable )
	void BossTakeDamaged( int32 damage );


	UPROPERTY( EditAnywhere )
	int32 BossMaxHP = 10;

	UPROPERTY( EditAnywhere )
	int32 BossHP = BossMaxHP;

	UPROPERTY( EditAnywhere )
	bool bIsAttacked = false;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	bool bIsDie = false;
};
