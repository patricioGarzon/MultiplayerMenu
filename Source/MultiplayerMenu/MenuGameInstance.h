// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "CoreMinimal.h"


#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "steam_api.h"
#include "USoundManagerData.h"

// The generated header should always be the last include
#include "MenuGameInstance.generated.h"

class UMainMenuWidget;
class UUSteamManagerSubsytem;
class USessionManagerSubsystem;
/**
 * 
 */


struct FSettingEntry;

UCLASS()
class MULTIPLAYERMENU_API UMenuGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	UFUNCTION()
	void OnMapLoaded(UWorld* LoadedWorld);
	void CheckForLobbyMap();

	// Spawning UI 
	void OpenMainMeu();

	UPROPERTY()
	TArray<FSettingEntry> GameSavedSettings;

	// Functions to create and populate main menu data
	UFUNCTION(BlueprintCallable)
	void CreateMainMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level UI")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
	UPROPERTY()
	UMainMenuWidget* MenuUI = nullptr;


	// Loading and saving games
	UFUNCTION(BlueprintCallable)
	void CreateGame();

	UFUNCTION(BlueprintCallable)
	TArray<FString> LoadSavedGames();

	UFUNCTION(BlueprintCallable)
	void SaveGame(FString FileName);

	//Sounds in game
	UPROPERTY(EditAnywhere)
	UUSoundManagerData* SoundData;

	//STEAM SUBSYSTEMS
	UUSteamManagerSubsytem* SteamManager = nullptr;
	USessionManagerSubsystem* SessionManager = nullptr;

private:

	float PlaySessionStartTime;
};
