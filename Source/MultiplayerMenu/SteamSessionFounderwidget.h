// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SteamSessionFounderwidget.generated.h"

class UVerticalBox;
class UClickeableSessionsInstance;
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API USteamSessionFounderwidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level UI")
	TSubclassOf<UClickeableSessionsInstance> SessionBTNClass = nullptr;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* SessionsContainer;

	void PopulateSteamSessions(const TArray<FOnlineSessionSearchResult>& Sessions);
private:

};
