// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIVA_API UBossHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY( EditAnywhere , meta = (BindWidget) )
	class UProgressBar* BossHPBar;

	void SetBosstHP( int32 BosstHP , int32 BossMaxHP );
};
