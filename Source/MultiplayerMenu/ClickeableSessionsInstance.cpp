// Fill out your copyright notice in the Description page of Project Settings.


#include "ClickeableSessionsInstance.h"
#include "OnlineSessionSettings.h"
#include "SessionManagerSubsystem.h"

void UClickeableSessionsInstance::NativeConstruct()
{
	Super::NativeConstruct();
	if (MenuButton)
	{
		MenuButton->OnClicked.AddDynamic(this, &UClickeableSessionsInstance::OnClickBtn);
		// If valid, bind the button click event

	}
}

void UClickeableSessionsInstance::SetSessionBtn(int maxPL, int curPL, FString map, FString owner, bool isPrivate)
{
	MaxPlayers = maxPL;
	CurPlayers = curPL;
	MapName = map;
	OwnerName = owner;
	canJoin = isPrivate;
	SetUI();
}

void UClickeableSessionsInstance::SetSessionDetails(const FOnlineSessionSearchResult& InSession)
{
	SessionsDetails = InSession;
}


void UClickeableSessionsInstance::OnClickBtn()
{
	//Get subsystem and call join
	if (UWorld* World = GetWorld())
	{
		USessionManagerSubsystem* SessionSubsystem = World->GetGameInstance()->GetSubsystem<USessionManagerSubsystem>();
		if (SessionSubsystem)
		{
			SessionSubsystem->AttemptJoinSession(SessionsDetails);
		}
	}
}

void UClickeableSessionsInstance::SetUI()
{
}


