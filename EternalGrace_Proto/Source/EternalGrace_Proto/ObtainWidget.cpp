// Fill out your copyright notice in the Description page of Project Settings.


#include "ObtainWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UObtainWidget::InitializeWidget(FName ObjectIDName, TSoftObjectPtr<UTexture2D> ThumpNail)
{
	ObjectID = ObjectIDName;
	ItemName->SetText(FText::FromName(ObjectID));
	ItemImage->SetBrushFromTexture(ThumpNail.LoadSynchronous());
}
