// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "CustomButton.h"
#include "UIEnums.h"
#include <Components/WidgetSwitcher.h>
#include <Components/Image.h>


class UMenuGameInstance;
#include <OnlineSessionSettings.h>
#include "MainMenuWidget.generated.h"

/**
 * 
 */


UCLASS()
class MULTIPLAYERMENU_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	FString steamID;
	FString steamNickname;

public:
	
	virtual void NativeConstruct() override;
	void PopulateSteamDetails();
	void SetSteamAvatar();
	UFUNCTION()
	void CreateGameSession();

	void OnCreateSessionComplete(FNamedOnlineSession* SessionName, bool bWasSuccessful);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMenuGameInstance* cachedGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "steam UI")
	UTexture2D* steamUserAvatar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SteamAvatar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	UCustomButton* BTN_CreateSession;

	UPROPERTY( meta = (BindWidget))
	UCustomButton* BTN_JoinSession;

	UPROPERTY( meta = (BindWidget))
	UCustomButton* BTN_Settings;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* PanelSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SteamNameText;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateSession;

	UFUNCTION()
	void OnMenuClicked(EMenuTypes MenuType);

	
};
