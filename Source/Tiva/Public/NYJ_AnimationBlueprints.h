// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NYJ_AnimationBlueprints.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNYJ_AnimationBlueprints : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TIVA_API INYJ_AnimationBlueprints
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetBowOnHand(bool BowOnHand) = 0;
	virtual void SetAim(bool Aim) = 0;
};
