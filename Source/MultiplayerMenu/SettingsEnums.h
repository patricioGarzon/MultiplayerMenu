// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESettingProfile : uint8
{
    Low			UMETA(DisplayName = "Low Quality"),
    Medium		UMETA(DisplayName = "Medium Quality"),
    High		UMETA(DisplayName = "High Quality"),
    Ultra		UMETA(DisplayName = "Ultra Quality"),
    Default		UMETA(DisplayName = "Default Quality")

};
UENUM(BlueprintType)
enum class ESettingsCategory : uint8
{
    GENERAL         UMETA(DisplayName = "GENERAL"),
    VIDEO       UMETA(DisplayName = "VIDEO"),
    SOUND        UMETA(DisplayName = "SOUND"),
    BINDINGS     UMETA(DisplayName = "BINDINGS"),
    VOICE        UMETA(DisplayName = "VOICE")

};

UENUM(BlueprintType)
enum class ESettingButtonType : uint8
{
    VideoSettings,
    CheckboxSettings,
    NumeralSettings
};
