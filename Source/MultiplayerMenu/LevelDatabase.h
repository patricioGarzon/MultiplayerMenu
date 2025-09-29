// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UIEnums.h"
#include "LevelDatabase.generated.h"

USTRUCT(BlueprintType)
struct FLevelData
{

	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> LevelPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> LevelImagePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EDifficulties> Difficulties;

};
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API ULevelDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Level Data")
	TArray<FLevelData> levels;
};
