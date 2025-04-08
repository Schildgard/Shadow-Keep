// Fill out your copyright notice in the Description page of Project Settings.


#include "DespawningValueBarWidget.h"

UDespawningValueBarWidget::UDespawningValueBarWidget()
{
	Countdown = DespawnTime;
}

void UDespawningValueBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsInViewport())
	{
		if (Countdown > 0)
		{
			Countdown -= InDeltaTime;
		}
		else
		{
			Countdown = DespawnTime;
			RemoveFromViewport();
		}
	}
}

void UDespawningValueBarWidget::ResetDespawnTimer()
{
	Countdown = DespawnTime;
}
