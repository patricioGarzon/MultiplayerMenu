// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySettingsWidget.h"

void ULobbySettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (BTN_CreateSession)
	{
		BTN_CreateSession->OnCustomButtonClicked.AddDynamic(this, &ULobbySettingsWidget::OnSessionCreatedCallback);
	}
}

void ULobbySettingsWidget::OnSessionCreatedCallback(EMenuTypes ButtonType)
{
	if (ButtonType == EMenuTypes::None) {
		int MyInt = 1;
		if (PlayerCountText->GetText().IsNumeric())
		{
			MyInt = static_cast<int>(FCString::Atoi(*PlayerCountText->GetText().ToString()));
		}
		OnCreateSessionClicked.Broadcast(LobbyNameText->GetText().ToString(), PasswordText->GetText().ToString(), MyInt, true, true);
	}
}
