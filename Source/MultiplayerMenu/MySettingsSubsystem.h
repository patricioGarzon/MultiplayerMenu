// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MySettingsSubsystem.generated.h"

struct FSettingEntry;
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API UMySettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SaveGameSettings();

	UFUNCTION()
	TArray<FSettingEntry> LoadGameSettings();

	UPROPERTY()
	TArray<FSettingEntry> CurrentSettings;
};
