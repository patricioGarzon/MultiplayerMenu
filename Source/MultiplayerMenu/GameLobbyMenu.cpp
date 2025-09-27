// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLobbyMenu.h"
#include "CustomButton.h"
#include <Kismet/GameplayStatics.h>
#include "MenuGameInstance.h"

void UGameLobbyMenu::NativeConstruct()
{
	BindButtons();
}

void UGameLobbyMenu::BindButtons()
{
	BackButton->OnCustomButtonClicked.AddDynamic(this, &UGameLobbyMenu::BackToMainMenu);
}

void UGameLobbyMenu::BackToMainMenu(EMenuTypes MenuType)
{
	UMenuGameInstance* GI = Cast<UMenuGameInstance>(GetWorld()->GetGameInstance());
	if (GI) {
		GI->OpenMainMeu();
	}
}
