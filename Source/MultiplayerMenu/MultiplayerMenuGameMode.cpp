// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerMenuGameMode.h"
#include "MultiplayerMenuCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerMenuGameMode::AMultiplayerMenuGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AMultiplayerMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

}
