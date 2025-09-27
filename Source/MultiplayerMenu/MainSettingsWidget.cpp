// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSettingsWidget.h"
#include "SettingsBaseButton.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "SettingsDataTable.h"
#include "SettingsSubsystem.h"
#include "SavedSettings.h"
#include "MenuGameInstance.h"
#include "CustomButton.h"

void UMainSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//read from file and create the array 
	if (!DT_GameSettings) return;
	LoadCategoryButtons();
	//check if data exist from game instance
	bFileExist = LoadOptionSettings();

	CacheData();
	
	FilterSettings(ESettingsCategory::GENERAL);
}

void UMainSettingsWidget::LoadCategoryButtons()
{
	UEnum* EnumPtr = StaticEnum<ESettingsCategory>();
	if (!EnumPtr) return;

	int32 EnumCount = EnumPtr->NumEnums() - 1; // Skip _MAX
	for (int32 n = 0; n < EnumCount; ++n)
	{
		if (EnumPtr->HasMetaData(TEXT("Hidden"), n))
			continue;
		if (BtnCategoryClass) {
			UCustomButton* TempBtn = CreateWidget<UCustomButton>(GetWorld(), BtnCategoryClass);
			if (TempBtn && OptionsPanel)
			{
				FString Name = EnumPtr->GetDisplayNameTextByIndex(n).ToString();
				TempBtn->FBName = Name;

				TempBtn->OnSettingButtonClicked.AddDynamic(this, &UMainSettingsWidget::FilterSettings);
				OptionsPanel->AddChild(TempBtn);
			}

		}
	}
}

void UMainSettingsWidget::UpdateSettingsVisuals(const TArray<FSettingMeta>& FilterSettings)
{
	//this is for creating the options itself
	int size = FilterSettings.Num();
	for (int n = 0; n < size; n++) {
		//create the settings place holder for each
		FSettingEntry* SavedEntry = GameSavedSettings.FindByPredicate([&](const FSettingEntry& Entry) {
			return Entry.Name == FilterSettings[n].OptionName.ToString();
		});
		if (BtnSettingClass) {
			USettingsBaseButton* tempBtn = CreateWidget<USettingsBaseButton>(GetWorld(), BtnSettingClass.Get());
			if (tempBtn) {
				tempBtn->OnBtnHovered.AddDynamic(this, &UMainSettingsWidget::PopulateDescription);
				if (bFileExist) {
					tempBtn->SetUpButton(FilterSettings[n].OptionName.ToString(),
						FilterSettings[n].Description,
						SavedEntry->bSavedBoolValue,
						FilterSettings[n].SettingType,
						FilterSettings[n].Commands,
						SavedEntry->Value);
				}
				else {
					tempBtn->SetUpButton(FilterSettings[n].OptionName.ToString(),
						FilterSettings[n].Description,
						FilterSettings[n].bIsActive,
						FilterSettings[n].SettingType,
						FilterSettings[n].Commands,
						FilterSettings[n].DefaultValue);


					//here we should get form the file and modify only the fields user modified
					//GameSavedSettings is TArray<FSettingEntry> 
				}
			}
		}
	}
}

void UMainSettingsWidget::CacheData()
{
	//get all rows from the data table
	TArray<FSettingsDataTable*> AllRows;
	CategorizedSettingsMap.Empty();
	DT_GameSettings->GetAllRows<FSettingsDataTable>(TEXT("Context"), AllRows);
	int size = AllRows.Num();
	//Iterate through each data asset form the table
	for (FSettingsDataTable* Row : AllRows)
	{
		if (!Row || !Row->VideoProfile)
			continue;

		UUVideoSettingAsset* VideoAsset = Row->VideoProfile;
		if (!VideoAsset) continue;

		for (const FSettingMeta& SettingData : VideoAsset->Settings)
		{
			CategorizedSettingsMap.FindOrAdd(VideoAsset->Category).Add(SettingData);
		}
	}
}


void UMainSettingsWidget::PopulateDescription(FString Data)
{
	if (!Data.IsEmpty() && OptionDescriptionText) {
		OptionDescriptionText->SetText(FText::FromString(Data));
	}
}

void UMainSettingsWidget::FilterSettings(ESettingsCategory Category)
{
	TArray<FSettingMeta> FilterSettings;
	if (const TArray<FSettingMeta>* VideoSettings = CategorizedSettingsMap.Find(Category))
	{
		for (const FSettingMeta& Setting : *VideoSettings)
		{
			FilterSettings.Add(Setting);
		}
	}

	UpdateSettingsVisuals(FilterSettings);
}

void UMainSettingsWidget::SetSettings()
{
	TArray<FSettingEntry> Entries;

	for (UWidget* Option : OptionsPanel->GetAllChildren())
	{
		USettingsBaseButton* Temp = Cast<USettingsBaseButton>(Option);
		if (Temp)
		{
			FSettingEntry TempEntry;
			TempEntry.Name = Temp->GetSettingName();
			Entries.Add(TempEntry);
		}
	}

}

bool UMainSettingsWidget::LoadOptionSettings()
{
	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (GI) {
		UMenuGameInstance* MGI = Cast<UMenuGameInstance>(GI);
		if (MGI) {
			if (!MGI->GameSavedSettings.IsEmpty()) {
				GameSavedSettings = MGI->GameSavedSettings;
				return true;
			}
		}
	}
	return false;
}


