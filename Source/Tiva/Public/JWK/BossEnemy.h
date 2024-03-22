// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tiva/TivaCharacter.h"
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
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class UStaticMeshComponent* swordComp;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class USphereComponent* attackSphereComp;


	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UBossFSM* bossFSM;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UCharacterMovementComponent* movementComp;


	//////////////////////////////////////// Damaged ////////////////////////////////////////
	UFUNCTION(BlueprintCallable)
	void BossTakeDamaged(int32 damage);

	UFUNCTION(Server , Reliable)
	void ServerBossTakeDamged(int32 damage);

	UFUNCTION(NetMulticast , Reliable)
	void MultiBossTakeDamged(int32 hp);

	UPROPERTY(EditAnywhere)
	int32 BossMaxHP = 12;

	UPROPERTY(Replicated , EditAnywhere)
	int32 BossHP = BossMaxHP;
	
	UPROPERTY(Replicated , EditAnywhere)
	bool bIsAttacked = false;

	UPROPERTY(Replicated , EditAnywhere)
	bool bIsDie = false;


	//////////////////////////////////////// Boss Widget ////////////////////////////////////////
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UWidgetComponent* bossHealthUI;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UBossHPWidget* bossHPWidget;

	UFUNCTION()
	void MakeBilboard();


	//////////////////////////////////////// 플레이어 대미지 처리 ////////////////////////////////////////
	UPROPERTY(EditAnywhere)
	class ATivaCharacter* player;

	////////////////////////////////////////////// NetWork //////////////////////////////////////////////
	UFUNCTION()
	void FindChoosePlayer();

	UPROPERTY(Replicated , EditAnywhere)
	class AActor* playerTarget;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
