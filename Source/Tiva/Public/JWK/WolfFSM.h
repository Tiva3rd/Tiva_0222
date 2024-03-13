// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WolfFSM.generated.h"

UENUM( BlueprintType )
enum class EWolf : uint8
{
	IDLE			 UMETA( DisplayName = "IDLE" ) ,
	MOVE			 UMETA( DisplayName = "MOVE" ) ,
	ATTACK			 UMETA( DisplayName = "ATTACK" ) ,
	DAMAGED			 UMETA( DisplayName = "DAMAGED" ) ,
	DEAD			 UMETA( DisplayName = "DEAD" )
};

UCLASS( ClassGroup = (Custom) , meta = (BlueprintSpawnableComponent) )
class TIVA_API UWolfFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWolfFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY()
	class AActor* playerTarget;

	UPROPERTY( EditAnywhere )
	class AWolf* me;

	UPROPERTY( EditAnywhere )
	class UWolfAnim* wolfAnim;

	UPROPERTY( EditAnywhere , BlueprintReadOnly )
	EWolf state;

	UPROPERTY()
	class AAIController* ai;

	UPROPERTY( EditAnywhere )
	float attackDistance = 200;
	UPROPERTY( EditAnywhere )
	float chasePlayerReach = 1500;

	UPROPERTY()
	FVector destinationToPlayer;
	float distanceToPlayer;

	void TickIdle();
	void TickMove();
	void TickAttack();
	void TakeDamage( int32 damage );
	void DoDamageEnd();
	void TickDead();

	float curTime;
	float attackDelayTime = 1.5f;

	UPROPERTY( EditAnywhere )
	class UAnimMontage* wolfMontage;

	void SetState( EWolf next );
};
