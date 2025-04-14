// Fill out your copyright notice in the Description page of Project Settings.
#include "CustromPlayerController.h"
#include "MenuGameInstance.h"


void ACustromPlayerController::BeginPlay()
{
	Super::BeginPlay();

    if (UMenuGameInstance* GI = Cast<UMenuGameInstance>(GetGameInstance()))
    {
        GI->CreateMenu(); // Safe to call here, PC and world are valid
    }
}
