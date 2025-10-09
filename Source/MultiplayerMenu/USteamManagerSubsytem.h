// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Toggle this ON if you're using a backend server for Steam ticket validation
#define USE_STEAM_AUTH_TICKET false
#define STEAM_SUBSYSTEM TEXT("Steam")


#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemSteam.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "steam_api.h"
#include "USoundManagerData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "USteamManagerSubsytem.generated.h"
/**
 * 
 */


UCLASS()
class MULTIPLAYERMENU_API UUSteamManagerSubsytem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public : 
	

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Get or load avatar
	UTexture2D* GetOrLoadAvatar();

	void CacheSteamSession(FString SessionName, FString SessionPassword, int MaxPlayers, bool JoinInProgress, bool ShouldAdvertise);
	//
	// Post-login callback handler
	void OnSteamLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	IOnlineSubsystem* GetSteamOnlineSubs() { return OnlineSubsystem; }
	IOnlineIdentityPtr GetIdentityInterface() { return IdentityInterface; }

	//get player Data
	FString GetSteamNickname() const { return CachedNickname; }
	FString GetSteamIdString() const { return CachedSteamId; }
private:
	UPROPERTY()
	UTexture2D* CachedPlayerAvatar;

	void CacheSteamUserInfo();

	IOnlineSubsystem* OnlineSubsystem;
	IOnlineSessionPtr SessionInterface;
	IOnlineIdentityPtr IdentityInterface;


	FString CachedNickname;
	FString CachedSteamId;
};
