// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsBaseButton.h"
#include <Components/Button.h>

void USettingsBaseButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (SettingButton) {
		SettingButton->OnClicked.AddDynamic(this, &USettingsBaseButton::HandleOneParamClick);
		SettingButton->OnHovered.AddDynamic(this, &USettingsBaseButton::OnHovered);
		SettingButton->OnUnhovered.AddDynamic(this, &USettingsBaseButton::OnUnHovered);

	}
}
void USettingsBaseButton::SetUpButton(FString Name, FString description, bool Active, ESettingsCategory btnClass)
{
	SettingName = Name;
	SettingDescription = description;
	SettingActive = Active;
	BTNCategory = btnClass;
}

void USettingsBaseButton::HandleOneParamClick()
{
	OnBtnClicked.Broadcast(BTNCategory);
}

void USettingsBaseButton::OnHovered()
{
	OnBtnHovered.Broadcast(SettingDescription);
	SettingButton->SetRenderOpacity(1);
	//increase opacity of the button, add extra animations
}

void USettingsBaseButton::OnUnHovered()
{
	//Reduce opacity of the btn textrue 
	SettingButton->SetRenderOpacity(0.7);
}
