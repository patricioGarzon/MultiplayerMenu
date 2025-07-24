// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingsEnums.h"
#include "SavedSettings.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSettingEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString Name;

    UPROPERTY(BlueprintReadWrite)
    int Value;

    UPROPERTY()
    bool bSavedBoolValue = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESettingProfile SettingProfile = ESettingProfile::Default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESettingsCategory Category = ESettingsCategory::GENERAL;
};


UCLASS()
class MULTIPLAYERMENU_API USavedSettings : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FSettingEntry> SavedSettings;


};
