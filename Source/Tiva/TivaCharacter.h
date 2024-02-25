// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "NYJ_AnimationBlueprints.h" 
#include "TivaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMyCustomDelegate);

UCLASS(config=Game)
class ATivaCharacter : public ACharacter, public INYJ_AnimationBlueprints
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	ATivaCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void JumpPlayer();

	void StopJumpingPlayer();
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//활 쏘기

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Aim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Aimable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BowOnHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* StandingDiveForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* StandingEquipBow;

	void ActionRightPressed();
	void ActionRightReleased();

	void ActionLeftPressed();
	void ActionLeftReleased();

	void AimBow();
	void RelaseBow();

public:
	virtual void SetBowOnHand(bool bBowOnHand) override;
	virtual void SetAim(bool bAim) override;

	//UFUNCTION(BlueprintCallable)
	//void StopRelease();

public:
	// 이벤트 델리게이트를 선언합니다.
	UPROPERTY(BlueprintAssignable)
	FMyCustomDelegate StopRelease;

	// 이벤트를 호출하는 멤버 함수를 정의합니다.
	UFUNCTION(BlueprintCallable)
	void CallStopRelease();

};

