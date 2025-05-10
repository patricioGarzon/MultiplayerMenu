// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomButton.h"
#include "LobbySettingsButton.generated.h"

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
	TObjectPtr<UTextBlock> LabelText;

	UFUNCTION()
	void UpdateText();
};
