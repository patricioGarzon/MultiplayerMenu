// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuWidget.h"
#include "MultiplayerMenuGameMode.generated.h"

UCLASS(minimalapi)
class AMultiplayerMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerMenuGameMode();
	virtual void BeginPlay() override;
};



