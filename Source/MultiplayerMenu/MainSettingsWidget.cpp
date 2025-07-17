// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSettingsWidget.h"
#include "SettingsBaseButton.h"
#include "Components/EditableText.h"
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

void UMainSettingsWidget::UpdateSettingsVisuals(const TArray<FSettingsDataTable>& FilterSettings)
{
	int size = FilterSettings.Num();
	for (int n = 0; n < size; n++) {
		//create the settings place holder for each
		UUserWidget* BtnSetting = CreateWidget(GetWorld(), BtnClass.Get());
		if (BtnSetting) {
			USettingsBaseButton* tempBtn = Cast<USettingsBaseButton>(BtnClass);
			if (tempBtn) {
				tempBtn->OnBtnHovered.AddDynamic(this, &UMainSettingsWidget::PopulateDescription);
				tempBtn->SetUpButton(FilterSettings[n].OptionName.ToString(),
					FilterSettings[n].OptionDescription.ToString(),
					FilterSettings[n].IsChecked,
					FilterSettings[n].Category);
			}
		}
	}
}

void UMainSettingsWidget::CacheData()
{
	//cach all player data for future use
	DT_GameSettings->GetAllRows<FSettingsDataTable>(TEXT("FetchAllSettings"), CachedSettings);
}


void UMainSettingsWidget::PopulateDescription(FString Data)
{
	if (!Data.IsEmpty()) {
		OptionDescriptionText->SetText(FText::FromString(Data));
	}
}

void UMainSettingsWidget::FilterSettings(ESettingsCategory Category)
{
	TArray<FSettingsDataTable> FilterSettings;
	int size = CachedSettings.Num();
	for (int n = 0; n < size; n++) {
		if (CachedSettings[n]->Category == Category) {
			FilterSettings.Add(*CachedSettings[n]);
		}
	}
	UpdateSettingsVisuals(FilterSettings);
}


