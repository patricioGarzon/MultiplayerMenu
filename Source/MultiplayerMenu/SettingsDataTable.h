// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UVideoSettingAsset.h"
#include "SettingsDataTable.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct MULTIPLAYERMENU_API FSettingsDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	FText OptionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
    FText OptionDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
    UUVideoSettingAsset* VideoProfile = nullptr;
};
