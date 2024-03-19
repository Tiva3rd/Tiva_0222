// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TivaLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TIVA_API ATivaLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
//	ATivaLobbyGameMode();

	virtual void BeginPlay() override;
	
};
