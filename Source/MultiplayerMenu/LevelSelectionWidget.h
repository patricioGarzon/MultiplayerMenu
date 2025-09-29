// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIEnums.h"
#include "LevelSelectionWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelSelected, const FLevelData&, SelectedLevel);

/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API ULevelSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetUpUI(const TArray<EDifficulties>& LevelDifficulties);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelSelected OnLevelSelected;
};
