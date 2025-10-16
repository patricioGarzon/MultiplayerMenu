// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomButton.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "ClickeableSessionsInstance.generated.h"


/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API UClickeableSessionsInstance : public UCustomButton
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetSessionBtn(int maxPL,int curPL, FString map, FString owner, bool isPrivate );

	void SetSessionDetails(const FOnlineSessionSearchResult& InSession);
	UFUNCTION()
	void OnClickBtn();
	UFUNCTION()
	void SetUI();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* lbl_lMapName = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* lbl_PlayersCount = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* lbl_Latency = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* lbl_OwnerName = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* lbl_Privacy = nullptr;


	FOnlineSessionSearchResult SessionsDetails;

private:
	int MaxPlayers;
	int CurPlayers;
	FString MapName;
	FString OwnerName;
	bool canJoin = false;


};
