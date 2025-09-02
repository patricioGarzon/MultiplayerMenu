// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsBaseButton.generated.h"

class UTextBlock;
class UButton;
enum class ESettingButtonType : uint8;

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
	UButton* SettingButton = nullptr;

	UFUNCTION()
	void SetUpButton(FString Name, FString description, bool Active, ESettingButtonType btnClass,const TArray<FString> Commands,int profileValue);

	UPROPERTY(BlueprintCallable)
	FOnSettingHovered OnBtnHovered;


	//Getters
	FString GetSettingName() { return SettingName; };
	TArray<FString> GetCommands() { return SettingCommands; };
protected:
	UFUNCTION()
	void ExecuteCommad();

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnHovered();
	UFUNCTION()
	void OnUnHovered();
private:
	FString SettingName;
	FString SettingDescription;
	TArray<FString> SettingCommands;
	bool SettingActive = false;
	ESettingButtonType BTNCategory;
	int SettingValue= 0;
};
