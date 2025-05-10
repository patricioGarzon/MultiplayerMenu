// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include "Components/EditableText.h"

#include "CustomButton.h"
#include "LobbySettingsWidget.generated.h"

/**
 * 
 */
UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnCreateSession,
	FString, SessionName,
	FString, SessionPassword,
	int32, MaxPlayers,
	bool, JoinInProgress,
	bool, ShouldAdvertise
);

UCLASS()
class MULTIPLAYERMENU_API ULobbySettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	 
	UFUNCTION(BlueprintCallable)
	void OnSessionCreatedCallback(EMenuTypes MenuType);

	//Delegates 
	UPROPERTY(BlueprintCallable)
	FOnCreateSession OnCreateSessionClicked;

	//Layout references

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableText* LobbyNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableText* PasswordText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableText* PlayerCountText;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCustomButton* BTN_CreateSession;
};
