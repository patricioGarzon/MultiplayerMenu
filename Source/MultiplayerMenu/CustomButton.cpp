// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomButton.h"


void UCustomButton::NativeConstruct()
{
    Super::NativePreConstruct();

    // Check if MenuButton and BTN_Text are valid
    if (MenuButton)
    {
        // If valid, bind the button click event
        MenuButton->OnClicked.AddDynamic(this, &UCustomButton::HandleClick);
    }

    // Check if BTN_Text is valid and update the text if needed
    if (BTN_Text)
    {
        BTN_Text->SetText(FText::FromString(FBName));
    }
}

void UCustomButton::HandleClick()
{
    OnCustomButtonClicked.Broadcast(BTN_Type);
}
