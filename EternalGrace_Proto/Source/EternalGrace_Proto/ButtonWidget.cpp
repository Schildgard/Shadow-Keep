// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonWidget.h"
#include "Components/Button.h"

void UButtonWidget::NativeConstruct()
{
	bIsFocusable = true;
	bIsEnabled = true;
	SetDesiredFocusWidget(Button);
}

void UButtonWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	//Play Sound and show hovered Outline
	RegisterSelection();
}

void UButtonWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	//remove hovered Outline
	UE_LOG(LogTemp, Error, TEXT("Removed from Focus"));
}

UButton* UButtonWidget::GetWidgetButton()
{
	return Button;
}

void UButtonWidget::RegisterSelection()
{
	if(SelectionSound)
	{
		PlaySound(SelectionSound);
	}
}
