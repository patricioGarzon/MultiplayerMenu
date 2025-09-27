// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "CustomButton.h"
#include "UIEnums.h"
#include <Components/WidgetSwitcher.h>
#include <Components/Image.h>
#include <OnlineSessionSettings.h>
class UMenuGameInstance;
class ULobbySettingsWidget;
class UMediaPlayer;
class UFileMediaSource;
class UNewsFeedManager;
struct FNewsItem;

#include "MainMenuWidget.generated.h"

/**
 * 
 */


UCLASS()
class MULTIPLAYERMENU_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	FString steamID;
	FString steamNickname;

public:
	virtual void NativeConstruct() override;

	//FUNTCIONTS -- Steam Related 
	void SetSteamAvatar();
	void PopulateSteamDetails();

	//FUNTCIONTS -- Steam Session 
	UFUNCTION()
	void ShowMainMenu(EMenuTypes MenuType);
	UFUNCTION()
	void CacheSession(FString SessionName, FString SessionPassword, int MaxPlayers, bool JoinInProgress, bool ShouldAdvertise);
	

	void OnCreateSessionComplete(FNamedOnlineSession* SessionName, bool bWasSuccessful);

	//FUNTCIONTS --News Feed
	UNewsFeedManager* GetNewsFeedManager();
	UFUNCTION(BlueprintImplementableEvent)
	void PopulateNewsFeed(const TArray<FNewsItem>& newsFeed);
	UFUNCTION(BlueprintImplementableEvent)
	void SwitchNews(int indexDif);

	//FUNTCIONTS -- Menu bindings
	UFUNCTION()
	void OnMenuClicked(EMenuTypes MenuType);
	UFUNCTION(BlueprintImplementableEvent)
	void PopulateLoadGames();
	UFUNCTION()
	void BindUI();
	

	// PROPERTIES -- Instances
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMenuGameInstance* cachedGameInstance;

	// PROPERTIES -- STEAM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "steam UI")
	UTexture2D* steamUserAvatar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SteamAvatar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SteamNameText;

	//Widgets 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	ULobbySettingsWidget* SessionSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUserWidget* WBP_NewsFeed;

	// PROPERTIES -- MEDIA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	UFileMediaSource* MediaSource;



	//PROPERTIES -- BUTTONS

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	UCustomButton* BTN_Play;

	UPROPERTY( meta = (BindWidget))
	UCustomButton* BTN_JoinGame;

	UPROPERTY( meta = (BindWidget))
	UCustomButton* BTN_Settings;

	UPROPERTY(meta = (BindWidget))
	UCustomButton* BTN_Quit;

	UPROPERTY(meta = (BindWidget))
	UCustomButton* BTN_Back;

	//PROPERTIES -- WIDGETS 

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* PanelSwitcher;

	//Other 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TArray<FString> SavedGamesArray;

};
