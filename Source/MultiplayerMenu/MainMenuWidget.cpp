// Fill out your copyright notice in the Description page of Project Settings.
#include "MainMenuWidget.h"
#include <Kismet/KismetSystemLibrary.h>
#include "MenuGameInstance.h"
#include "LobbySettingsWidget.h"
#include <OnlineSessionSettings.h>


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//make sure button is not  a null ptr
	if (BTN_CreateSession != nullptr) {
		BTN_CreateSession->OnCustomButtonClicked.AddDynamic(this, &UMainMenuWidget::OnMenuClicked);
	}
	if (BTN_JoinSession != nullptr) {
		BTN_JoinSession->OnCustomButtonClicked.AddDynamic(this, &UMainMenuWidget::OnMenuClicked);
	}
	if (BTN_Settings != nullptr) {
		BTN_Settings->OnCustomButtonClicked.AddDynamic(this, &UMainMenuWidget::OnMenuClicked);
	}	
	if (SessionSettings && SessionSettings->BTN_CreateSession)
	{
		// Bind the OnClicked event to your function
		SessionSettings->OnCreateSessionClicked.AddDynamic(this, &UMainMenuWidget::CacheSession);
	}
	PopulateSteamDetails();
	SetSteamAvatar();
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
	switch (MenuType) {
	case EMenuTypes::Play:
		PanelSwitcher->SetActiveWidgetIndex(0);		
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
