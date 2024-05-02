// Copyright Epic Games, Inc. All Rights Reserved.

#include "BAFG_CaliburstGameMode.h"
#include "BAFG_CaliburstCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABAFG_CaliburstGameMode::ABAFG_CaliburstGameMode()
{
	player1 = nullptr;
	player2 = nullptr;
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}
