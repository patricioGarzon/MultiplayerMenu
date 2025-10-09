// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelSelectionWidget.h"
#include "LevelDatabase.h"
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/UniformGridPanel.h>
#include "ClickableImage.h"


void ULevelSelectionWidget::SetUpUI(ULevelDatabase* Data)
{
	if (!Data)return;

	gridData = Data;
	if (!gridData->levels.IsEmpty()) {
		PupulateLevels();
		SetLevelDescription(0);
	}
	
}

void ULevelSelectionWidget::PupulateLevels()
{
	if (!btnClass.IsValid())
	{
		UClass* LoadedClass = btnClass.LoadSynchronous();
		if (!LoadedClass)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load UClickableImage class!"));
			return;
		}
		btnClass = LoadedClass; // optional: now IsValid() returns true
	}
	int size = gridData->levels.Num();
	int rows = 0;
	int col = 0;
	for (int n = 0; n < size; n++)
	{
		UClickableImage* mapLvl = CreateWidget<UClickableImage>(this, btnClass.Get());
		if (!mapLvl) continue;

		mapLvl->Index = n;
		mapLvl->OnBtnClick.AddDynamic(this, &ULevelSelectionWidget::SetLevelDescription);
		mapLvl->SetBtnImage(gridData->levels[n].LevelImagePath.LoadSynchronous());

		mapLvl->SetVisibility(ESlateVisibility::Visible); // make sure it's visible

		LevelGrid->AddChildToUniformGrid(mapLvl, rows, col);

		col++;
		if (col >= DesiredRows)
		{
			col = 0;
			rows++;
		}

		//Set first item selected 
		if (n == 0) {
			mapLvl->b_IsSelected = true;
			mapLvl->HandleBtnClick();
		}
	}
}

void ULevelSelectionWidget::SetLevelDescription(int index)
{
	if (MissionDescription && !gridData->levels[index].LevelDescription.IsEmpty()) {
		MissionDescription->SetText(FText::FromString(gridData->levels[index].LevelDescription));
		curIndex = index;
	}
	
	if (LevelGrid && LevelGrid->HasAnyChildren()) {
		int size = LevelGrid->GetChildrenCount();
		for (int n = 0; n < size; n++) {
			if (n != index) {
				Cast<UClickableImage>(LevelGrid->GetChildAt(n))->HandleDeselect();
			}
		}
	}
}
