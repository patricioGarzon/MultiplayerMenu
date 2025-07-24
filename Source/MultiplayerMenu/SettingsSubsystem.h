// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SavedSettings.h"
#include "UVideoSettingAsset.h"
#include "SettingsSubsystem.generated.h"
/**
 * 
 */


UCLASS()
class MULTIPLAYERMENU_API USettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	UFUNCTION()
	void SaveGameSettings(TArray<FSettingEntry> CurrentSettings);

	UFUNCTION()
	TArray<FSettingEntry> LoadGameSettings();

	UFUNCTION()
	void ApplyCurSettings(TArray<FSettingMeta> CurrentSettings);

};
