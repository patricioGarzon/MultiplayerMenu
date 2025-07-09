// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSettingsWidget.h"
#include "SettingsBaseButton.h"
#include "Blueprint/UserWidget.h"
#include "SettingsDataTable.h"

void UMainSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//read from file and create the array 
	if (!DT_GameSettings) return;

	CacheData();
	FilterSettings(ESettingsCategory::GENERAL);
}

void UMainSettingsWidget::UpdateSettingsVisuals(TArray<FSettingsDataTable> FilterSettings)
{
	int size = FilterSettings.Num();
	for (int n = 0; n < size; n++) {
		//create the settings place holder for each
		UUserWidget* BtnSetting = CreateWidget(GetWorld(), BtnClass.Get());

	}
}

void UMainSettingsWidget::CacheData()
{
	//cach all player data for future use
	DT_GameSettings->GetAllRows<FSettingsDataTable>(TEXT("FetchAllSettings"), CachedSettings);
}


void UMainSettingsWidget::FilterSettings(ESettingsCategory Category)
{

}


