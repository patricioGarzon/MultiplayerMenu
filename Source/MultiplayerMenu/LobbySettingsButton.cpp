// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySettingsButton.h"

void ULobbySettingsButton::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULobbySettingsButton::UpdateText()
{
	//Logic for changing the Text
	if (LabelText) {
		LabelText->SetText(FText::FromString("Test"));
	}
}
