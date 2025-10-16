// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <Interfaces/OnlineSessionDelegates.h>
#include "OnlineSessionSettings.h"
#include "SessionManagerSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSessionsFound, const TArray<FOnlineSessionSearchResult>&);

class FOnlineSessionSearch;
class UUSteamManagerSubsytem;
///STRUCT FOR SESSION SETTINGS
USTRUCT(BlueprintType)
struct FSessionDetails
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString SessionName;
	UPROPERTY(BlueprintReadWrite)
	FString SessionPassword;
	UPROPERTY(BlueprintReadWrite)
	int MaxPlayers = 0;
	UPROPERTY(BlueprintReadWrite)
	bool JoinInProgress = false;
	UPROPERTY(BlueprintReadWrite)
	bool ShouldAdvertise = false;
};
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API USessionManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Connectivity hosting and finding game;
	UPROPERTY(BlueprintReadWrite)
	FSessionDetails ChSessionDetails;
	void CacheSession(FString SessionName, FString SessionPassword, int MaxPlayers, bool JoinInProgress, bool ShouldAdvertise);
	void CreateSession();
	void FindSessions();
	void AttemptJoinSession(FOnlineSessionSearchResult &inSession);

	//For Testing UI
	void CreateDummySessions();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// Sessions Call backs 
// Delegate to handle session creation completion
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;	

	void OnFindSessionsComplete(bool bWasSuccessful);

	FOnSessionsFound OnSessionsFound;

private:
	//STEAM SUBSYSTEMS
	UUSteamManagerSubsytem* SteamManager = nullptr;
	bool bIsLan = false;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

};
