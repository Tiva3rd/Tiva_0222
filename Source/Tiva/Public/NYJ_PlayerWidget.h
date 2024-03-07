// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NYJ_PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIVA_API UNYJ_PlayerWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UProgressBar* Bar_HP;

	UPROPERTY( EditAnywhere, meta = (BindWidget) )
	class UTextBlock* TestTextBlock;
	
	//UPROPERTY( EditDefaultsOnly , BlueprintReadWrite )
	//float hp = 1.0f;
};
