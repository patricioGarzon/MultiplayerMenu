// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "CoreMinimal.h"
// For multiplayer 
#include "OnlineSubsystem.h"
#include "OnlineSubsystemSteam.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "steam_api.h"
#include "USoundManagerData.h"

// The generated header should always be the last include
#include "MenuGameInstance.generated.h"

class UMainMenuWidget;
class UUSteamManagerSubsytem;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FSessionDetails
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	FString SessionName;
	UPROPERTY(BlueprintReadWrite)
	FString SessionPassword;
	UPROPERTY(BlueprintReadWrite)
	int MaxPlayers =0;
	UPROPERTY(BlueprintReadWrite)
	bool JoinInProgress = false;
	UPROPERTY(BlueprintReadWrite)
	bool ShouldAdvertise = false;
};

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

	// Connectivity hosting and finding game;
	UPROPERTY(BlueprintReadWrite)
	FSessionDetails ChSessionDetails;
	void CacheSession(FString SessionName, FString SessionPassword, int MaxPlayers, bool JoinInProgress, bool ShouldAdvertise);
	void CreateSession();
	void FindSessions();
	void JoinSession();


	// Loading and saving games
	UFUNCTION(BlueprintCallable)
	void CreateGame();

	UFUNCTION(BlueprintCallable)
	TArray<FString> LoadSavedGames();

	UFUNCTION(BlueprintCallable)
	void SaveGame(FString FileName);

			// Sessions Call backs 
	// Delegate to handle session creation completion
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	//functions to handle callbacks 
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	//Sounds in game
	UPROPERTY(EditAnywhere)
	UUSoundManagerData* SoundData;

	UUSteamManagerSubsytem* SteamManager;

private:

	float PlaySessionStartTime;
};
