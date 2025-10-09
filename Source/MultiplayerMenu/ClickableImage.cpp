// Fill out your copyright notice in the Description page of Project Settings.
#include "ClickableImage.h"
#include <Components/Button.h>
#include <Components/Image.h>

void UClickableImage::NativeConstruct()
{
	BindBtnMethods();
	
}

void UClickableImage::BindBtnMethods()
{
	if (ClickableBTN) {
		ClickableBTN->OnHovered.AddDynamic(this, &UClickableImage::HandleHovered);
		ClickableBTN->OnUnhovered.AddDynamic(this, &UClickableImage::HandleUnhovered);
		ClickableBTN->OnClicked.AddDynamic(this, &UClickableImage::HandleBtnClick);
	}
}

void UClickableImage::HandleHovered()
{
	ModifyBtnOpacity(1.0f);
}

void UClickableImage::HandleUnhovered()
{
	if (!b_IsSelected) {
		ModifyBtnOpacity(0.7);
	}	
}

void UClickableImage::HandleBtnClick()
{
	OnBtnClick.Broadcast(Index);
	b_IsSelected = true;
	ModifyBtnOpacity(1.0f);
	BackgroundGlow->SetVisibility(ESlateVisibility::Visible);
}
void UClickableImage::HandleDeselect()
{
	b_IsSelected = false;
	ModifyBtnOpacity(0.7);
	BackgroundGlow->SetVisibility(ESlateVisibility::Collapsed);
}


void UClickableImage::ModifyBtnOpacity(float opc)
{
	if (BackGroundImg)
	{
		BackGroundImg->SetRenderOpacity(opc);
	}
}

void UClickableImage::SetBtnImage(UTexture2D* img)
{
	if (BackGroundImg) {
		BackGroundImg->SetBrushFromTexture(img);
	}	
}
