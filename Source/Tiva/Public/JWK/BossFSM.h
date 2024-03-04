// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossFSM.generated.h"


UENUM(BlueprintType)
enum class EBoss_Enemy : uint8
{
	IDLE			 UMETA(DisplayName = "IDLE"),
	MOVE			 UMETA(DisplayName = "MOVE"),
	ATTACKPLAYER     UMETA(DisplayName = "ATTACKPlAYER"),
	ATTACKHOME		 UMETA(DisplayName = "ATTACKHOME"),
	ATTACKWAIT		 UMETA(DisplayName = "ATTACKWAIT"),
	DEAD			 UMETA(DisplayName = "DEAD")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TIVA_API UBossFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBossFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 목적지를 테스트 Pawn --> 배치되어있는 Pawn 클래스 FSM Detail 에서 수정
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class APawn* mainTarget;

	UPROPERTY()
	class AActor* playerTarget;

	UPROPERTY(EditAnywhere)
	class ABossEnemy* me;

	UPROPERTY(EditAnywhere)
	class UBossAnim* bossAnim;

	// Boss Enemy의 State
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBoss_Enemy state;

	// Boss Enemy 의 AI
	UPROPERTY()
	class AAIController* ai;




	// Boss Attack Range
	UPROPERTY(EditAnywhere)
	float attackDistance = 200;
	UPROPERTY(EditAnywhere)
	float chasePlayerReach = 800;



private:
	// Boss Enemy State Function
	void TickIdle();
	void TickMove();
	void TickAttackHome();
	void TickAttackPlayer();
	void TickDamage();
	void TakeDamage(int damage);
	void TickDead();
	void DoDamageEnd();

	float curTime;
	float attackDelayTime = 3.0f;

public:
	void SetState(EBoss_Enemy next);

};
