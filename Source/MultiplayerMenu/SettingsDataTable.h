// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SettingsDataTable.generated.h"

UENUM(BlueprintType)
enum class ESettingsCategory : uint8
{
    GENERAL         UMETA(DisplayName = "GENERAL"),
    VIDEO       UMETA(DisplayName = "VIDEO"),
    SOUND        UMETA(DisplayName = "SOUND"),
    BINDINGS     UMETA(DisplayName = "BINDINGS"),
    VOICE        UMETA(DisplayName = "VOICE")

};
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
    ESettingsCategory Category = ESettingsCategory::GENERAL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
    FText OptionDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
    bool IsChecked = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
    FText OptionCommand;
};
