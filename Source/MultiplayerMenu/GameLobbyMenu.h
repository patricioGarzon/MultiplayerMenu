// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIEnums.h"
#include "GameLobbyMenu.generated.h"

class UCustomButton;
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
	ULobbySettingsButton* Map = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	ULobbySettingsButton* Difficulty = nullptr;

private:
	void BindButtons();

	UFUNCTION()
	void BackToMainMenu(EMenuTypes MenuType);
};
