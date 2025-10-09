// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLobbyMenu.h"
#include "CustomButton.h"
#include <Kismet/GameplayStatics.h>
#include "MenuGameInstance.h"
#include "LevelSelectionWidget.h"
#include <Components/Button.h>
#include <Components/ComboBoxString.h>
#include "LobbySettingsButton.h"

void UGameLobbyMenu::NativeConstruct()
{
	BindButtons();
	SetUIData();
}

void UGameLobbyMenu::BindButtons()
{
	if (BackButton) {
		BackButton->OnCustomButtonClicked.AddDynamic(this, &UGameLobbyMenu::BackToMainMenu);
	}
	if (b_GameSettings) {
		b_GameSettings->OnCustomButtonClicked.AddDynamic(this, &UGameLobbyMenu::DeployMapSelectionWidget);
	}
	if (LevelSelection) {
		//LevelSelection->AcceptButton.AddDynamic(this, &UgameLobbyMenu::HandleMapSelected);
		LevelSelection->AcceptButton->OnClicked.AddDynamic(this, &UGameLobbyMenu::HandleMapSelected);
	}
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
		SetMapData(0);	
		LevelSelection->SetUpUI(LevelDatabase);
	}
}

void UGameLobbyMenu::HandleMapSelected()
{
	SetMapData(LevelSelection->curIndex);
	LevelSelection->SetVisibility(ESlateVisibility::Collapsed);
}

void UGameLobbyMenu::DeployMapSelectionWidget(EMenuTypes MenuType)
{
	LevelSelection->SetVisibility(ESlateVisibility::Visible);
}


void UGameLobbyMenu::SetMapData(int index)
{
	if (LevelDatabase->levels[index].LevelImagePath.IsValid()) {
		//load the image
		UTexture2D* thumbnail = LevelDatabase->levels[index].LevelImagePath.LoadSynchronous();
		MapPreview->SetBrushFromTexture(thumbnail);
	}
	if (!LevelDatabase->levels[index].LevelName.IsEmpty()) {
		b_GameSettings->MapName->SetText(FText::FromString(LevelDatabase->levels[index].LevelName));
	}
	SetComboBoxData(LevelDatabase->levels[index].Difficulties);
}

void UGameLobbyMenu::SetComboBoxData(const TArray<EDifficulties>& LevelDifficulties)
{
	b_GameSettings->Difficulties->ClearOptions();
	UEnum* EnumPtr = StaticEnum<EDifficulties>();
	for (EDifficulties df : LevelDifficulties) {
		FText DisplayName = EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(df));
		b_GameSettings->Difficulties->AddOption(DisplayName.ToString());
	}
	b_GameSettings->Difficulties->SetSelectedIndex(0);
}