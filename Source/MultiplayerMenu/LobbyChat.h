// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyChat.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API ULobbyChat : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ChatTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SendMessageBTN = nullptr;

private:
	FString SteamID;
	FString ChatMessage;
};
