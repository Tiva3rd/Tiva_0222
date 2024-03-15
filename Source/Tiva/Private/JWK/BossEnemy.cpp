// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossEnemy.h"

#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JWK/BossFSM.h"
#include "EngineUtils.h"
#include "JWK/BossAnim.h"
#include "Net/UnrealNetwork.h"
#include "JWK/BossHPWidget.h"
#include "JWK/WolfAnim.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tiva/TivaCharacter.h"

// Sets default values
ABossEnemy::ABossEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//UCapsuleComponent* MyCapsuleComponent = GetCapsuleComponent();
	//MyCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// BossHPUI 생성
	bossHealthUI = CreateDefaultSubobject<UWidgetComponent>( TEXT( "bossHealthUI" ) );
	bossHealthUI->SetupAttachment( RootComponent );
	bossHealthUI->SetRelativeLocation( FVector( 10 , 0 , 135 ) );

	bossFSM = CreateDefaultSubobject<UBossFSM>( TEXT( "bossFSM" ) );

	swordComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "swordComp" ) );
	swordComp->SetupAttachment( GetMesh() , TEXT( "hand_rSocket" ) );

	attackSphereComp = CreateDefaultSubobject<USphereComponent>( TEXT( "attackSphereComp" ) );
	attackSphereComp->SetupAttachment( swordComp );
	attackSphereComp->SetWorldScale3D( FVector( 8.0f ) );

	movementComp = CreateDefaultSubobject<UCharacterMovementComponent>( TEXT( "movementComp" ) );

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempSword( TEXT( "/Script/Engine.StaticMesh'/Game/00_JWK/Blueprint/BP_Enemy/Boss/AatroxSword.AatroxSword'" ) );

	if (tempSword.Succeeded())
	{
		swordComp->SetStaticMesh( tempSword.Object );
		swordComp->SetRelativeLocationAndRotation( FVector( 190 , -20 , 5 ) , FRotator( 90 , 0 , 90 ) );
		swordComp->SetWorldScale3D( FVector( 0.25f ) );
	}

	bReplicates = true;
	SetReplicateMovement( true );
}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	NetUpdateFrequency = 100;

	bossHPWidget = Cast<UBossHPWidget>( bossHealthUI->GetWidget() );
}

// Called every frame
void ABossEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	MakeBilboard();
	OnRep_FindPlayer();
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
	auto anim = Cast<UBossAnim>( GetMesh()->GetAnimInstance() );

	if (BossHP <= 0)
	{
		bIsDie = true;

		BossHP = 0;

		anim->PlayDeathAnimation();
		GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	}

	if (BossHP > 0)
	{
		anim->PlayHitAnimation();
		//GetCharacterMovement()->SetMovementMode( MOVE_None );
	}

	bossHPWidget->SetBosstHP( BossHP , BossMaxHP );
}

////////////////////////////////// 보스 체력바 빌보드 처리 //////////////////////////////////
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

void ABossEnemy::OnRep_FindPlayer()
{
	// 레벨에 있는 ATivaCharacter 객체들을 다 검사해서 chasePlayerReach안에 있고
	// 그중에서도 가장 까가운녀석을 내 playerTarget으로 지정
	if (HasAuthority())
	{
		playerTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
		float tempDist = bossFSM->chasePlayerReach;


		for (TActorIterator<ATivaCharacter> IT( GetWorld() ); IT; ++IT)
		{
			ATivaCharacter* newPlayerCharacter = *IT;

			// 플레이어와 나의 거리를 측정해서
			float temp = newPlayerCharacter->GetDistanceTo( this );

			// 만약 temp가 tempDist보다 가깝다면
			if (temp <= tempDist)
			{
				// NewPlayer로 기억하고싶다.
				playerTarget = newPlayerCharacter;

				// tempDist = temp;
				tempDist = temp;
			}
		}
	}
}

void ABossEnemy::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( ABossEnemy , BossHP );
	DOREPLIFETIME( ABossEnemy , bIsDie );
	DOREPLIFETIME( ABossEnemy , playerTarget );
}

//void ABossEnemy::DealDamage()
//{
//	TArray<TEnumAsByte<EObjectTypeQuery>> QueryArray;
//	TArray<AActor*> IgnoreActors;
//	TArray<AActor*> OutActors;
//	UKismetSystemLibrary::SphereOverlapActors( GetWorld() , FVector(/*attackSphereComp->GetComponentLocation()*/) , 100 , QueryArray , APawn::StaticClass() , IgnoreActors , OutActors );
//
//	UKismetSystemLibrary::DrawDebugSphere( GetWorld() , attackSphereComp->GetComponentLocation() , 100 , 12 , FLinearColor::Red , 2 , 0 );
//
//	for (int i = 0; i < OutActors.Num(); i++)
//	{
//		player->PlayerTakeDamaged(5);
//	}
//}
