// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsBaseButton.h"
#include "SettingsEnums.h"
#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>

void USettingsBaseButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (SettingButton) {

		SettingButton->OnHovered.AddDynamic(this, &USettingsBaseButton::OnHovered);
		SettingButton->OnUnhovered.AddDynamic(this, &USettingsBaseButton::OnUnHovered);

	}
}
void USettingsBaseButton::SetUpButton(FString Name, FString description, bool Active, ESettingButtonType btnClass,const TArray<FString> Commands,int profileValue)
{
	SettingName = Name;
	SettingDescription = description;
	SettingActive = Active;
	BTNCategory = btnClass;
	SettingCommands = Commands;
	SettingValue = profileValue;
}

void USettingsBaseButton::ExecuteCommad()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{	
		for (FString cmnd : SettingCommands) {
			if (!cmnd.IsEmpty()) {
				PC->ConsoleCommand(cmnd, true);
			}
		}
	}
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
