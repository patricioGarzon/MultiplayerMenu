// Fill out your copyright notice in the Description page of Project Settings.
#include "MainMenuWidget.h"
#include <Kismet/KismetSystemLibrary.h>
#include "MenuGameInstance.h"
#include "LobbySettingsWidget.h"
#include <OnlineSessionSettings.h>
#include "MultiplayerMenuGameMode.h"
#include "NewsFeedManager.h"
#include <Kismet/GameplayStatics.h>


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PopulateSteamDetails();
	SetSteamAvatar();
	BindUI();

	if (UNewsFeedManager* FeedManager = GetNewsFeedManager()) {
		FeedManager->OnNewsUpdated.AddDynamic(this, &UMainMenuWidget::PopulateNewsFeed);		
	}

	if (auto* GM = Cast< AMultiplayerMenuGameMode>(UGameplayStatics::GetGameMode(this))) {
		GM->LoadNews();
	}
	if (cachedGameInstance) {
		SavedGamesArray = cachedGameInstance->LoadSavedGames();
		PopulateLoadGames();
	}

}

UNewsFeedManager* UMainMenuWidget::GetNewsFeedManager()
{
	//access the news feed manager to retrieve the data;
	if (auto* GM = Cast< AMultiplayerMenuGameMode>(UGameplayStatics::GetGameMode(this)) ){
		return GM->curNewsGameManager;
	}
	return nullptr;
}




void UMainMenuWidget::PopulateSteamDetails()
{
	if (GetWorld()) {
		if (UGameInstance* GI = GetWorld()->GetGameInstance()) {
			cachedGameInstance = Cast<UMenuGameInstance>(GI);
			if (cachedGameInstance) {
				steamNickname = cachedGameInstance->GetSteamNickname();
				steamID = cachedGameInstance->GetSteamIdString();
				if (SteamNameText) {
					SteamNameText->SetText(FText::FromString(steamNickname));
				}
			}
		}
	}
}

void UMainMenuWidget::SetSteamAvatar()
{
	
	if (SteamAvatar && steamUserAvatar)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(steamUserAvatar);
		Brush.ImageSize = FVector2D(steamUserAvatar->GetSizeX(), steamUserAvatar->GetSizeY());

		SteamAvatar->SetBrush(Brush);
	}	
}

void UMainMenuWidget::ShowMainMenu(EMenuTypes MenuType)
{
	PanelSwitcher->SetActiveWidgetIndex(0);
	BTN_Back->SetVisibility(ESlateVisibility::Collapsed);
	WBP_NewsFeed->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::CacheSession(FString SessionName, FString SessionPassword, int MaxPlayers, bool JoinInProgress, bool ShouldAdvertise)
{
	cachedGameInstance->CacheSession(SessionName,SessionPassword, MaxPlayers, JoinInProgress,ShouldAdvertise);
	cachedGameInstance->CreateSession();
}

void UMainMenuWidget::OnCreateSessionComplete(FNamedOnlineSession* CreatedSession, bool bWasSuccessful)
{
	int32 MaxPlayers = CreatedSession->SessionSettings.NumPublicConnections;
	int32 OpenSlots = CreatedSession->NumOpenPublicConnections;
	int32 CurrentPlayers = MaxPlayers - OpenSlots;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, "Session Created");
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		FString::Printf(TEXT("Session Details: Name: %s, Players: %d/%d"),
			*CreatedSession->SessionName.ToString(),
			CurrentPlayers,
			MaxPlayers));
}


void UMainMenuWidget::OnMenuClicked(EMenuTypes MenuType)
{
	if (MenuType != EMenuTypes::Quit) {
		WBP_NewsFeed->SetVisibility(ESlateVisibility::Hidden);
		BTN_Back->SetVisibility(ESlateVisibility::Visible);
	}	
	switch (MenuType) {
	case EMenuTypes::Play:
		PanelSwitcher->SetActiveWidgetIndex(1);		
		break;
	case EMenuTypes::CreateSession:
		PanelSwitcher->SetActiveWidgetIndex(0);
		SetSteamAvatar();
		break;
	case EMenuTypes::JoinSession:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, "Find session BTN");
		PanelSwitcher->SetActiveWidgetIndex(1);
		break;
	case EMenuTypes::Quit:
		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
		break;

	}
}


void UMainMenuWidget::BindUI()
{
	//make sure button is not  a null ptr
	if (BTN_Play != nullptr) {
		BTN_Play->OnCustomButtonClicked.AddDynamic(this, &UMainMenuWidget::OnMenuClicked);
	}
	if (BTN_JoinGame != nullptr) {
		BTN_JoinGame->OnCustomButtonClicked.AddDynamic(this, &UMainMenuWidget::OnMenuClicked);
	}
	if (BTN_Settings != nullptr) {
		BTN_Settings->OnCustomButtonClicked.AddDynamic(this, &UMainMenuWidget::OnMenuClicked);
	}
	if (BTN_Quit != nullptr) {
		BTN_Quit->OnCustomButtonClicked.AddDynamic(this, &UMainMenuWidget::OnMenuClicked);
	}
	if (SessionSettings && SessionSettings->BTN_CreateSession)
	{
		// Bind the OnClicked event to your function
		SessionSettings->OnCreateSessionClicked.AddDynamic(this, &UMainMenuWidget::CacheSession);
	}
	if (BTN_Back != nullptr) {
		BTN_Back->OnCustomButtonClicked.AddDynamic(this, &UMainMenuWidget::ShowMainMenu);
	}
}

