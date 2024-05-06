// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BAFG_CaliburstCharacter.h"
#include "BAFG_CaliburstGameMode.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	VE_Default	UMETA(DisplayName = "Mannequin"),
	VE_Char		UMETA(DisplayName = "Char"),
	VE_Ako		UMETA(DisplayName = "Ako")
};

UCLASS(minimalapi)
class ABAFG_CaliburstGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABAFG_CaliburstGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ABAFG_CaliburstCharacter* player1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ABAFG_CaliburstCharacter* player2;
};



