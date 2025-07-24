// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsSubsystem.h"
#include <Kismet/GameplayStatics.h>


void USettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Called automatically at game startup (once the GameInstance is ready)
    UE_LOG(LogTemp, Log, TEXT("SettingsSubsystem Initialized"));

    // Example: load settings from file
    LoadGameSettings();
}

void USettingsSubsystem::Deinitialize()
{
    Super::Deinitialize();
    UE_LOG(LogTemp, Log, TEXT("SettingsSubsystem Deinitialized"));
}

void USettingsSubsystem::SaveGameSettings(TArray<FSettingEntry> CurrentSettings)
{
    USavedSettings* SaveGameInstance = Cast<USavedSettings>(UGameplayStatics::CreateSaveGameObject(USavedSettings::StaticClass()));

    SaveGameInstance->SavedSettings = CurrentSettings;

    UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("GameSettings"), 0);
}

TArray<FSettingEntry> USettingsSubsystem::LoadGameSettings()
{

    TArray<FSettingEntry> Result;
    //Check if file exist and load from there otherwise load from data table 
    if (UGameplayStatics::DoesSaveGameExist(TEXT("GameSettings"), 0))
    {        
        USavedSettings* Loaded = Cast<USavedSettings>(UGameplayStatics::LoadGameFromSlot(TEXT("GameSettings"), 0));
        if (Loaded)
        {
            Result = Loaded->SavedSettings;
        }
    }
    return Result;
}

void USettingsSubsystem::ApplyCurSettings(TArray<FSettingMeta> CurrentSettings)
{
    //Retrieve the list and apply each command with the default value 
    ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(0);
    if (LocalPlayer && LocalPlayer->PlayerController) {
        for (FSettingMeta setting : CurrentSettings) {
            for (FString str : setting.Commands) {
                FString cmd = str.Append(" ");
                cmd.Append(FString::FromInt(setting.DefaultValue));

                LocalPlayer->ConsoleCommand(cmd);
            }
        }
    }    
}
