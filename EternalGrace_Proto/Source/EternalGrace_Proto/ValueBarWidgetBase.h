// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ValueBarWidgetBase.generated.h"

/**
 * 
 */
class UProgressBar;
UCLASS()
class ETERNALGRACE_PROTO_API UValueBarWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta =(BindWidget))
	UProgressBar* ProgressBar;
public:
	UFUNCTION()
	void UpdateProgressBar(float CurrentValue, float MaxValue);
	
};
