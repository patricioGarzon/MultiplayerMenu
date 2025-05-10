// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIEnums.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "CustomButton.generated.h"

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCustonBTNClicked, EMenuTypes, BTN_Type);

UCLASS()
class MULTIPLAYERMENU_API UCustomButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Btn Type")
	EMenuTypes BTN_MenuType;

	//Delegates declaration 
	UPROPERTY(BlueprintCallable)
	FOnCustonBTNClicked OnCustomButtonClicked;

	

	//UI BInding

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> MenuButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> BTN_Text;


	//Text binding

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Btn Type")
	FString FBName = " ";


	UFUNCTION(BlueprintCallable)
	void HandleOneParamClick();

};
