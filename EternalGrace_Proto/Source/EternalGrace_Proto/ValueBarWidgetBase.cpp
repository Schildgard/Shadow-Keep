// Fill out your copyright notice in the Description page of Project Settings.


#include "ValueBarWidgetBase.h"
#include "Components/ProgressBar.h"

void UValueBarWidgetBase::UpdateProgressBar(float CurrentValue, float MaxValue)
{
	ProgressBar->SetPercent(CurrentValue / MaxValue);
}
