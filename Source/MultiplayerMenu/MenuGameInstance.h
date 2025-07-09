// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
// Toggle this ON if you're using a backend server for Steam ticket validation
#define USE_STEAM_AUTH_TICKET false


#include "CoreMinimal.h"
// For multiplayer 
#include "OnlineSubsystem.h"
#include "OnlineSubsystemSteam.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "steam_api.h"
class UMainMenuWidget;

// The generated header should always be the last include
#include "MenuGameInstance.generated.h"

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

UCLASS()
class MULTIPLAYERMENU_API UMenuGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	UFUNCTION()
	void OnMapLoaded(UWorld* LoadedWorld);
	void CheckForLobbyMap();

	// Post-login callback handler
	void OnSteamLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	// Steam player infromation
	void PrintSteamInfo();
	FString GetSteamNickname() const { return CachedNickname; }
	FString GetSteamIdString() const { return CachedSteamId; }


	// Functions to create and populate main menu data
	UFUNCTION(BlueprintCallable)
	void CreateMainMenu();
	UTexture2D* GetSteamAvatar();
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
private:
	void CacheSteamUserInfo();

	IOnlineSubsystem* OnlineSubsystem;
	IOnlineSessionPtr SessionInterface;
	IOnlineIdentityPtr IdentityInterface;

	
	FString CachedNickname;
	FString CachedSteamId;

	float PlaySessionStartTime;
};
