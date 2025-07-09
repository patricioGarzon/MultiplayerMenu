// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySavedGame.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API UMySavedGame : public USaveGame
{
	GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    FString FileName;
    UPROPERTY(BlueprintReadWrite)
    FString MapName;

    UPROPERTY(BlueprintReadWrite)
    int32 Progression;

    UPROPERTY(BlueprintReadWrite)
    int32 SavedSlot;

    UPROPERTY(BlueprintReadWrite)
    float  TimePlayed;

};
