// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObtainContainerWidget.h"
#include "Components/StackBox.h"
#include "Components/Button.h"

void UItemObtainContainerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(!(RemoveWidgetButton->OnClicked.IsAlreadyBound(this, &UItemObtainContainerWidget::RemoveWidget)))
	{
	RemoveWidgetButton->OnClicked.AddDynamic(this, &UItemObtainContainerWidget::RemoveWidget);
	}
}

void UItemObtainContainerWidget::RemoveWidget()
{
	StackBox->ClearChildren();
	RemoveFromParent();
}
