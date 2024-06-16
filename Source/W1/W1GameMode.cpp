// Copyright Epic Games, Inc. All Rights Reserved.

#include "W1GameMode.h"
#include "W1Character.h"
#include "UObject/ConstructorHelpers.h"

AW1GameMode::AW1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
