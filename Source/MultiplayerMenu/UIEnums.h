// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIEnums.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMenuTypes : uint8
{
	Play		  UMETA(DisplayName = "Play"),
	CreateSession UMETA(DisplayName = "CreateSession"),
	JoinSession   UMETA(DisplayName = "JoinSession"),
	Quit   UMETA(DisplayName = "Quit")
};
