// Fill out your copyright notice in the Description page of Project Settings.
#include "CustromPlayerController.h"
#include "GameLobbyMenu.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include "MainMenuWidget.h"
#include "MenuGameInstance.h"


ACustromPlayerController::ACustromPlayerController()
{

}

void ACustromPlayerController::BeginPlay()
{
    Super::BeginPlay();
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, TEXT("PC begin play"));
    BindInputMapping();
}

void ACustromPlayerController::Client_ShowLobby_Implementation()
{
    
    // Ensure that the lobby widget class is valid
    if (LobbyWidgetClass)
    {
        ShowUI();
    }
    else
    {

        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("Too early to show UI"));
    }
}

void ACustromPlayerController::Server_TravelToLobby_Implementation()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("performing server travel"));
    GetWorld()->ServerTravel("/Game/Menu/LobbyMenuLevel?listen", true);
}

void ACustromPlayerController::ShowUI()
{
    // Log for debugging purposes
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("ShowUI called"));

    if (LobbyWidgetClass)
    {
        // Create the widget
        LobbyWidget = CreateWidget<UGameLobbyMenu>(this, LobbyWidgetClass.Get(), "UI menu");

        if (LobbyWidget)
        {
            // Add it to the viewport
            LobbyWidget->AddToViewport();
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("Lobby added to viewport"));

            // Set input mode to UI only
            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(LobbyWidget->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            SetInputMode(InputMode);
            bShowMouseCursor = true;
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("Failed to create Lobby widget"));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("Lobby Widget Class is not set"));
    }
}

void ACustromPlayerController::SwichNews(int index)
{
    UMenuGameInstance* GI = Cast<UMenuGameInstance>(GetGameInstance());
    if (GI) {
        if (GI->MenuUI) {
            GI->MenuUI->SwitchNews(index);
        }
    }
}
