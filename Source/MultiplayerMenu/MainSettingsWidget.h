// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "MainSettingsWidget.generated.h"


enum class ESettingsCategory : uint8;
enum class ESettingButtonType : uint8;
class UHorizontalBox;
class UTextBlock;
struct FSettingEntry;
struct FSettingsDataTable;
struct FSettingMeta;
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API UMainSettingsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BTNSetSettings = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* MainOptionsPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* OptionsPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* OptionDescriptionText = nullptr;;
	// Functions for retrieving infomration
	UFUNCTION()
	void LoadCategoryButtons();
	
	UFUNCTION()
	void UpdateSettingsVisuals(const TArray<FSettingMeta>& FilterSettings);

	UFUNCTION()
	void CacheData();

	void PopulateDescription(FString Data);
	UFUNCTION()
	void FilterSettings(ESettingsCategory Category);

	UFUNCTION()
	void SetSettings();

	UFUNCTION()
	bool LoadOptionSettings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> BtnSettingClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> BtnCategoryClass;

private:

	TMap<ESettingsCategory, TArray<FSettingMeta>> CategorizedSettingsMap;
	TArray<FSettingEntry> GameSavedSettings;
	bool bFileExist = false;
protected:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* DT_GameSettings = nullptr;
};
