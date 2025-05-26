// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NewsFeedManager.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewsUpdated, const TArray<FNewsItem>&, NewsList);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIPLAYERMENU_API UNewsFeedManager : public UActorComponent
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "News")
    void LoadFromDataAsset(UNewsFeedData* DataAsset);

    UFUNCTION(BlueprintCallable, Category = "News")
    void LoadFromJSON(const FString& FilePath); // Future extension


    UPROPERTY(BlueprintAssignable)
    FOnNewsUpdated OnNewsUpdated;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "News Feed")
    UNewsFeedData* NewsDataAsset;

private:
    TArray<FNewsItem> NewsItems;
};

