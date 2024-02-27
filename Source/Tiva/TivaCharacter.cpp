// Copyright Epic Games, Inc. All Rights Reserved.

#include "TivaCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <Kismet/KismetMathLibrary.h>
#include "Animation/AnimMontage.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATivaCharacter

ATivaCharacter::ATivaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATivaCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATivaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATivaCharacter::JumpPlayer);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATivaCharacter::StopJumpingPlayer);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATivaCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATivaCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	PlayerInputComponent->BindAction(TEXT("RightMouse"), EInputEvent::IE_Pressed, this, &ATivaCharacter::ActionRightPressed);

	PlayerInputComponent->BindAction(TEXT("RightMouse"), EInputEvent::IE_Released, this, &ATivaCharacter::ActionRightReleased);

	PlayerInputComponent->BindAction(TEXT("LeftMouse"), EInputEvent::IE_Pressed, this, &ATivaCharacter::ActionLeftPressed);

	PlayerInputComponent->BindAction(TEXT("LeftMouse"), EInputEvent::IE_Released, this, &ATivaCharacter::ActionLeftReleased);
}

void ATivaCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATivaCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller

		//화살들고 화면 회전할때 잘 움직이게 하기 위한 계산//
		AddControllerYawInput(LookAxisVector.X * UKismetMathLibrary::SelectFloat(0.4, 1.0, Aim));
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATivaCharacter::JumpPlayer()
{
	//if (!!ACharacter::IsPlayingRootMotion())
	//{
		//if (!!BowOnHand) {
			//ACharacter::Jump();
		//}
		//else {
			//PlayAnimMontage(StandingDiveForward);
		//}
	//}
}

void ATivaCharacter::StopJumpingPlayer()
{
	//if (!!ACharacter::IsPlayingRootMotion())
	//{
		//if (!!BowOnHand) {
			//ACharacter::StopJumping();
		//}
	//}
}

void ATivaCharacter::ActionRightPressed()
{
	//if (BowOnHand) {
	//	Aim = false;
	//	AimBow();
	//}
	//else {
		//BowOnHand = true;
		//Aimable = true;
		//AActor* Character = GetOwner();
		//INYJ_AnimationBlueprints* Interface = Cast<INYJ_AnimationBlueprints>(Character);
		//if (Interface != nullptr)
		//{
			//Interface->SetBowOnHand(BowOnHand);
		//}
		//PlayAnimMontage(StandingEquipBow);

		//UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();
		//if (CharacterMovement)
		//{
			// MaxWalkSpeed 속성 설정
			//CharacterMovement->MaxWalkSpeed = 125.0f;
		//}
		//if (Aimable) {
			//AimBow();
		//}
	//}
}

void ATivaCharacter::ActionRightReleased()
{

}

void ATivaCharacter::ActionLeftPressed()
{

}

void ATivaCharacter::ActionLeftReleased()
{
}

void ATivaCharacter::AimBow()
{


}

void ATivaCharacter::RelaseBow()
{

}
