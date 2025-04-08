// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ValueBarWidgetBase.h"
#include "DespawningValueBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALGRACE_PROTO_API UDespawningValueBarWidget : public UValueBarWidgetBase
{
	GENERATED_BODY()
protected:
	UDespawningValueBarWidget();

	
	const float DespawnTime = 3.0f;
	UPROPERTY()
	float Countdown;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

public:
	UFUNCTION()
	void ResetDespawnTimer();
};
