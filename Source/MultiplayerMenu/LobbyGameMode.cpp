// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "CustromPlayerController.h"

ALobbyGameMode::ALobbyGameMode()
	: Super()
{
   
    PlayerControllerClass = ACustromPlayerController::StaticClass(); // Make sure to set this!
    PrimaryActorTick.bCanEverTick = true;
}

void ALobbyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
}


void ALobbyGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALobbyGameMode::OnLevelLoaded()
{

}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

    if (ACustromPlayerController* PC = Cast<ACustromPlayerController>(NewPlayer))
    {
        if (GetWorld()->IsGameWorld())
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("We are on lobby game mode post login"));

            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle,
                PC,
                &ACustromPlayerController::Client_ShowLobby,
                1.0f,
                false
            );
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("NewPlayer is not ACustromPlayerController!"));
    }

}


