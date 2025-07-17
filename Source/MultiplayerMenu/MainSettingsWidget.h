// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainSettingsWidget.generated.h"

enum class ESettingsCategory : uint8;
class UHorizontalBox;
class UEditableText;
struct FSettingsDataTable;
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API UMainSettingsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUserWidget* MainOptionsPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* OptionsPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableText* OptionDescriptionText = nullptr;;
	// Functions for retrieving infomration
	UFUNCTION()
	void UpdateSettingsVisuals(const TArray<FSettingsDataTable>& FilterSettings);

	UFUNCTION()
	void CacheData();

	void PopulateDescription(FString Data);
	UFUNCTION()
	void FilterSettings(ESettingsCategory Category);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> BtnClass;
private:
	UDataTable* DT_GameSettings = nullptr;
	TArray<FSettingsDataTable*> CachedSettings;
protected:
	virtual void NativeConstruct() override;
};
