// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomButton.h"
#include "LobbySettingsButton.generated.h"

class UComboBoxString;
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API ULobbySettingsButton : public UCustomButton
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	//For settings in lobby 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LabelText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UComboBoxString* Difficulties = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MapName = nullptr;
	UFUNCTION()
	void UpdateText();

	UPROPERTY(EditAnywhere, Category = "Details")
	FString LabelName; 




};
