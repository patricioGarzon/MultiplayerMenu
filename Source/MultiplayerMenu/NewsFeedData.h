// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NewsFeedData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FNewsItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* LocalImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ImageURL; // for remote loading

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Timestamp; // Optional
};

UCLASS()
class MULTIPLAYERMENU_API UNewsFeedData : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FNewsItem> NewsItems;
};
