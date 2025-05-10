// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustromPlayerController.generated.h"

// Forward declaration of your Widget class
class UGameLobbyMenu;

UCLASS()
class MULTIPLAYERMENU_API ACustromPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ACustromPlayerController();
    // Client RPC to show the lobby
    UFUNCTION(Client, Reliable)
    void Client_ShowLobby();
    virtual void Client_ShowLobby_Implementation();

    UFUNCTION(Server, Reliable)
    void Server_TravelToLobby();

    // The class of the lobby widget to show (set from Blueprints or C++)
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSoftClassPtr <UGameLobbyMenu> LobbyWidgetClass;
protected:
    virtual void BeginPlay() override;

    // Handle showing the UI on client
    UFUNCTION(BlueprintCallable)
    void ShowUI();

private:


    // The widget itself
    UPROPERTY()
    UGameLobbyMenu* LobbyWidget;


    // Flag to track if the world has begun play
    bool bHasBegunPlay = false;

    // Timer handle to delay UI showing
    FTimerHandle MyTimerHandle;
};