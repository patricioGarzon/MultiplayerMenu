// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SettingsEnums.h"
#include "UVideoSettingAsset.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FSettingMeta
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName OptionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Commands;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    //Allows modifying the type of settings to retrieve info from
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESettingButtonType SettingType = ESettingButtonType::VideoSettings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SettingType == ESettingType::VideoSettings"))
    ESettingProfile SettingProfile = ESettingProfile::Default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SettingType == ESettingType::NumeralSettings"))
    int32 DefaultValue = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SettingType == ESettingType::CheckboxSettings"))
    bool bIsActive = false;
};

UCLASS()
class MULTIPLAYERMENU_API UUVideoSettingAsset : public UDataAsset
{
	GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSettingMeta> Settings;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESettingsCategory Category = ESettingsCategory::GENERAL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bModifyAll = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bModifyAll", EditConditionHides))
    int IOveralSettings = 2;
};
