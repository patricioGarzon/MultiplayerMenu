// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerMenuGameMode.h"
#include "MultiplayerMenuCharacter.h"
#include "NewsFeedManager.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerMenuGameMode::AMultiplayerMenuGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Templates/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AMultiplayerMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	curNewsGameManager = NewObject<UNewsFeedManager>(this);
	curNewsGameManager->RegisterComponent();
	
}

void AMultiplayerMenuGameMode::LoadNews()
{
	curNewsGameManager->LoadFromDataAsset(NewsDataAsset);
}
