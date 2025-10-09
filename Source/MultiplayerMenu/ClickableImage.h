// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClickableImage.generated.h"

class UButton;
class UImage;
/**
 * 
 */
UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClickableImgClicked, int, Index);

UCLASS()
class MULTIPLAYERMENU_API UClickableImage : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ClickableBTN = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* BackGroundImg = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundGlow = nullptr;

	void BindBtnMethods();
	UFUNCTION()
	void HandleHovered();
	UFUNCTION()
	void HandleUnhovered();

	UFUNCTION()
	void HandleBtnClick();

	UFUNCTION()
	void HandleDeselect();

	void ModifyBtnOpacity(float opc);
	void SetBtnImage(UTexture2D* img);
	UPROPERTY()
	UTexture2D* ImageTexture = nullptr;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FClickableImgClicked OnBtnClick;

	int Index = 0;
	bool b_IsSelected = false;
};
