// Copyright Epic Games, Inc. All Rights Reserved.

#include "Self_ReverseTimeGameMode.h"
#include "Self_ReverseTimeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASelf_ReverseTimeGameMode::ASelf_ReverseTimeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
