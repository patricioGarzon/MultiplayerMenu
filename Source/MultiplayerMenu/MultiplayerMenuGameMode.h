// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuWidget.h"
class UNewsFeedManager;
class UNewsFeedData;
#include "MultiplayerMenuGameMode.generated.h"

UCLASS(minimalapi)
class AMultiplayerMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerMenuGameMode();
	virtual void BeginPlay() override;

	void LoadNews();
	//News Feed Manager
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "News Feed")
	UNewsFeedManager* curNewsGameManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "News Feed")
	UNewsFeedData* NewsDataAsset;
};



