// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "USoundManagerData.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API UUSoundManagerData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase*> MenuTracks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase*> SFX;
};
