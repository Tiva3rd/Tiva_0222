// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoblinFSM.generated.h"

UENUM( BlueprintType )
enum class EGoblin_Enemy : uint8
{
	IDLE			 UMETA( DisplayName = "IDLE" ) ,
	MOVE			 UMETA( DisplayName = "MOVE" ) ,
	ATTACK			 UMETA( DisplayName = "ATTACK" ) ,
	DEAD			 UMETA( DisplayName = "DEAD" )
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIVA_API UGoblinFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoblinFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// 목적지를 테스트 Pawn --> 배치되어있는 Pawn 클래스 FSM Detail 에서 수정
	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class AhouseTargetColumn* mainTarget;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class AActor* playerTarget;

	UPROPERTY( EditAnywhere )
	class AGoblinEnemy* me;

	UPROPERTY( EditAnywhere )
	class UGoblinAnim* goblinAnim;

	// Goblin Enemy의 State
	UPROPERTY( EditAnywhere , BlueprintReadOnly )
	EGoblin_Enemy state;

	// Goblin Enemy 의 AI
	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class AAIController* ai;




	// Goblin Attack Range
	UPROPERTY( EditAnywhere )
	float attackDistance = 200;
	UPROPERTY( EditAnywhere )
	float chasePlayerReach = 1500;


	// Goblin Enemy State Function
	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDead();

	float curTime;
	float attackDelayTime = 3.5f;

	void SetState( EGoblin_Enemy next );
};
