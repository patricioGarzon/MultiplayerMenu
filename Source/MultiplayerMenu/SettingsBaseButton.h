// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsBaseButton.generated.h"

class UTextBlock;
class UButton;
enum class ESettingsCategory : uint8;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingsBTNClicked, ESettingsCategory, BTN_Type);

UDELEGATE(	)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingHovered, FString, Description);
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API USettingsBaseButton : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SettingsName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SettingsDescription = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton = nullptr;

	UFUNCTION()
	void SetUpButton(FString Name, FString description, bool Active, ESettingsCategory btnClass);

	UPROPERTY(BlueprintCallable)
	FOnSettingsBTNClicked OnBtnClicked;

	UPROPERTY(BlueprintCallable)
	FOnSettingHovered OnBtnHovered;
protected:
	UFUNCTION()
	void HandleOneParamClick();

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnHovered();
	UFUNCTION()
	void OnUnHovered();
private:
	FString SettingName;
	FString SettingDescription;
	bool SettingActive = false;
	ESettingsCategory BTNCategory;
};
