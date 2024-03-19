// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "TivaNetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TIVA_API UTivaNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	IOnlineSessionPtr sessionInterface;

	void CreateRoom( int32 maxPlayerCount , FString roomName );

	void OnMyCreateRoom( FName sessionName , bool bWasSuccessful );

	FString hostName = TEXT( "hello" );
	//FString roomName;
};
