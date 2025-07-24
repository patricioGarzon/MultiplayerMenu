// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomButton.h"


void UCustomButton::NativeConstruct()
{
    Super::NativePreConstruct();

    // Check if MenuButton and BTN_Text are valid
    if (MenuButton)
    {
         MenuButton->OnClicked.AddDynamic(this, &UCustomButton::HandleOneParamClick);
        // If valid, bind the button click event
        
    }

    // Check if BTN_Text is valid and update the text if needed
    if (BTN_Text && !FBName.IsEmpty())
    {
        BTN_Text->SetText(FText::FromString(FBName));
    }
}

void UCustomButton::HandleOneParamClick()
{
    OnCustomButtonClicked.Broadcast(BTN_MenuType);
}

void UCustomButton::HandleSettingsParamClick()
{
    OnSettingButtonClicked.Broadcast(SettingCategory);
}
