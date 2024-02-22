// Copyright Epic Games, Inc. All Rights Reserved.

#include "TivaGameMode.h"
#include "TivaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATivaGameMode::ATivaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
