// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossEnemy.h"

#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JWK/BossFSM.h"
#include "JWK/BossHPWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABossEnemy::ABossEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// BossHPUI 생성
	bossHealthUI = CreateDefaultSubobject<UWidgetComponent>( TEXT( "bossHealthUI" ) );
	bossHealthUI->SetupAttachment( RootComponent );
	bossHealthUI->SetRelativeLocation( FVector( 10 , 0 , 135 ) );

	bossFSM = CreateDefaultSubobject<UBossFSM>( TEXT( "bossFSM" ) );

	swordComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "swordComp" ) );
	swordComp->SetupAttachment( GetMesh() , TEXT( "hand_rSocket" ) );

	movementComp = CreateDefaultSubobject<UCharacterMovementComponent>( TEXT( "movementComp" ) );

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempSword( TEXT( "/Script/Engine.StaticMesh'/Game/00_JWK/Blueprint/BP_Enemy/Boss/AatroxSword.AatroxSword'" ) );

	if (tempSword.Succeeded())
	{
		swordComp->SetStaticMesh( tempSword.Object );
		swordComp->SetRelativeLocationAndRotation( FVector( 190 , -20 , 5 ) , FRotator( 90 , 0 , 90 ) );
		swordComp->SetWorldScale3D( FVector( 0.25f ) );
	}

}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	bossHPWidget = Cast<UBossHPWidget>( bossHealthUI->GetWidget() );
}

// Called every frame
void ABossEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	MakeBilboard();
}

// Called to bind functionality to input
void ABossEnemy::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

}


//////////////////////////////////////// 공격 당함 ////////////////////////////////////////
void ABossEnemy::BossTakeDamaged( int32 damage )
{
	BossHP -= damage;

	if (BossHP <= 0)
	{
		bIsDie = true;

		BossHP = 0;
	}

	bossHPWidget->SetBosstHP( BossHP , BossMaxHP );
}

void ABossEnemy::MakeBilboard()
{
	//카메라의 location과 UI의 location을 가져오고
	FVector cameraLoc = UGameplayStatics::GetPlayerCameraManager( this , 0 )->GetCameraLocation();
	FVector uiLoc = bossHealthUI->GetComponentLocation();

	//두 location을 빼서 normalize
	FVector tmp = cameraLoc - uiLoc;
	tmp.Normalize();
	//make rot from x 해서
	FRotator bilboardRotate = UKismetMathLibrary::MakeRotFromX( tmp );
	//UI의 rotation을 set
	bossHealthUI->SetWorldRotation( bilboardRotate );
}

