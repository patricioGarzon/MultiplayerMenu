// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIEnums.h"
#include "LevelDatabase.h"
#include <Components/Image.h>
#include "GameLobbyMenu.generated.h"



class UCustomButton;
class ULevelSelectionWidget;
class ULobbySettingsButton;
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API UGameLobbyMenu : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCustomButton* BackButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	ULobbySettingsButton* b_GameSettings = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	ULevelSelectionWidget* LevelSelection = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI Data")
	ULevelDatabase* LevelDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* MapPreview = nullptr;

private:
	UFUNCTION()
	void BindButtons();

	UFUNCTION()
	void BackToMainMenu(EMenuTypes MenuType);
	
	UFUNCTION()
	void SetUIData();

	UFUNCTION()
	void HandleMapSelected();

	UFUNCTION()
	void DeployMapSelectionWidget(EMenuTypes MenuType);

	void SetComboBoxData(const TArray<EDifficulties>& LevelDifficulties);

	void SetMapData(int Jindex);

};

