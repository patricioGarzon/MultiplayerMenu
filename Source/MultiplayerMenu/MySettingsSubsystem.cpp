// Fill out your copyright notice in the Description page of Project Settings.


#include "MySettingsSubsystem.h"
#include "SavedSettings.h"
#include <Kismet/GameplayStatics.h>

void UMySettingsSubsystem::SaveGameSettings()
{
	//Load the settings saved under saved folder
	if (UGameplayStatics::DoesSaveGameExist(TEXT("UserSettings"), 0)) {
		//if exist create an array of saved game entries for future use
		USavedSettings* SaveGameInstance = Cast<USavedSettings>(UGameplayStatics::CreateSaveGameObject(USavedSettings::StaticClass()));

		SaveGameInstance->SavedSettings = CurrentSettings;

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("UserSettings"), 0);
	}
}

TArray<FSettingEntry> UMySettingsSubsystem::LoadGameSettings()
{
    TArray<FSettingEntry> Result;

    if (UGameplayStatics::DoesSaveGameExist(TEXT("UserSettings"), 0))
    {
        USavedSettings* Loaded = Cast<USavedSettings>(UGameplayStatics::LoadGameFromSlot(TEXT("UserSettings"), 0));
        if (Loaded)
        {
            Result = Loaded->SavedSettings;
        }
    }

    return Result;
}
