// Fill out your copyright notice in the Description page of Project Settings.


#include "OJS/TivaLobbyGameMode.h"

void ATivaLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor( true );
}
