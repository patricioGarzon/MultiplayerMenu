// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLobbyMenu.h"
#include "CustomButton.h"
#include <Kismet/GameplayStatics.h>
#include "MenuGameInstance.h"
#include "LobbySettingsButton.h"

void UGameLobbyMenu::NativeConstruct()
{
	BindButtons();
	SetUIData();
}

void UGameLobbyMenu::BindButtons()
{
	BackButton->OnCustomButtonClicked.AddDynamic(this, &UGameLobbyMenu::BackToMainMenu);
	b_GameSettings->OnCustomButtonClicked.AddDynamic(this, &UGameLobbyMenu::MapSelection);
}

void UGameLobbyMenu::BackToMainMenu(EMenuTypes MenuType)
{
	UMenuGameInstance* GI = Cast<UMenuGameInstance>(GetWorld()->GetGameInstance());
	if (GI) {
		GI->OpenMainMeu();
	}
}

void UGameLobbyMenu::SetUIData()
{
	//check if level data exist
	if (LevelDatabase) {
		//first load first level
		SetMapData(LevelDatabase->levels[0]);				
	}
}

void UGameLobbyMenu::MapSelection(EMenuTypes MenuType)
{
	//open widget to choose levels

}

void UGameLobbyMenu::SetMapData(FLevelData Level)
{
	if (Level.LevelImagePath.IsValid()) {
		//load the image
		UTexture2D* thumbnail = Level.LevelImagePath.LoadSynchronous();
		MapPreview->SetBrushFromTexture(thumbnail);
	}
	if (!Level.LevelName.IsEmpty()) {
		b_GameSettings->MapName->SetText(FText::FromString(Level.LevelName));
	}
	SetComboBoxData(Level.Difficulties);
}

void UGameLobbyMenu::SetComboBoxData(const TArray<EDifficulties>& LevelDifficulties)
{

}