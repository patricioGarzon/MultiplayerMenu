// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIEnums.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EMenuTypes : uint8
{
	None		  UMETA(DisplayName = "No Menu"),
	Play		  UMETA(DisplayName = "Play"),
	CreateSession UMETA(DisplayName = "CreateSession"),
	JoinSession   UMETA(DisplayName = "Join Session"),
	Settings	  UMETA(DisplayName = "Settings"),
	Quit   UMETA(DisplayName = "Quit")
};

UENUM(BlueprintType)
enum class EButtonType : uint8
{
	OneParam		UMETA(DisplayName = "One param"),
	TwoParam		UMETA(DisplayName = "Two param"),
	ThreeParam		UMETA(DisplayName = "Three param"),
	FiveParam		UMETA(DisplayName = "Five param")
};

UENUM(BlueprintType)
enum class EDifficulties : uint8
{
	Easy			UMETA(DisplayName = "Easy"),
	Medium			UMETA(DisplayName = "Medium"),
	Hard			UMETA(DisplayName = "Hard"),
	Nightmare			UMETA(DisplayName = "Nightmare")
};

USTRUCT(BlueprintType)
struct FLevelData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> LevelImagePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDifficulties Difficulties = EDifficulties::Easy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EDifficulties> CurDifficulty;

};
