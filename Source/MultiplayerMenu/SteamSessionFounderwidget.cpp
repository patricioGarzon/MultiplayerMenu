// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamSessionFounderwidget.h"
#include "MenuGameInstance.h"
#include "SessionManagerSubsystem.h"

#include "Blueprint/UserWidget.h"     // for CreateWidget
#include "Components/VerticalBox.h"   // for UVerticalBox
#include "Engine/World.h"             // for GetWorld()
#include "ClickeableSessionsInstance.h" // your custom widget

void USteamSessionFounderwidget::NativeConstruct()
{
	Super::NativeConstruct();
	UMenuGameInstance* GI = Cast<UMenuGameInstance>(GetGameInstance());
	if (GI)
	{
		if (GI->SessionManager) {
			GI->SessionManager->OnSessionsFound.AddUObject(this, &USteamSessionFounderwidget::PopulateSteamSessions);
		}
	}
}

void USteamSessionFounderwidget::PopulateSteamSessions(const TArray<FOnlineSessionSearchResult>& Sessions)
{
	if (!Sessions.IsEmpty()) {
		int num = Sessions.Num();
		for (int i = 0; i < num; i++) {
			FName WidgetName = FName(*FString::Printf(TEXT("Session_%d"), i));

			// Use 'this' as context when inside a UUserWidget
			UClickeableSessionsInstance* SessionWidget =
				CreateWidget<UClickeableSessionsInstance>(this, SessionBTNClass, WidgetName);
			if (SessionWidget) {
				SessionsContainer->AddChildToVerticalBox(Cast<UUserWidget>(SessionWidget));
				//add on click delegate to join the session

			}
		}
	}
}
