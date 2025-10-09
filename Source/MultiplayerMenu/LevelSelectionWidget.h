// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIEnums.h"
#include "LevelSelectionWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;
class UUniformGridPanel;
class ULevelDatabase;
class UClickableImage;

/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API ULevelSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetUpUI(ULevelDatabase* Data);
	void PupulateLevels();

	UFUNCTION()
	void SetLevelDescription(int index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUniformGridPanel* LevelGrid = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MissionDescription = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* AcceptButton = nullptr;

	UPROPERTY()
	ULevelDatabase* gridData;

	UPROPERTY(EditAnywhere, Category = "Details")
	int DesiredRows = 2;

	UPROPERTY(EditAnywhere, Category = "LevelBtn Class")
	TSoftClassPtr<UClickableImage> btnClass;

	UPROPERTY()
	int curIndex = 0;
};
