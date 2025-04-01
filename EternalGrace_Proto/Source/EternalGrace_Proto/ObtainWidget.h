// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObtainWidget.generated.h"

/**
 * 
 */
class UButton;
class UImage;
class UTextBlock;
UCLASS()
class ETERNALGRACE_PROTO_API UObtainWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget), Transient)
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget), Transient)
	UImage* ItemImage;

public:
	UPROPERTY()
	FName ObjectID;
	UFUNCTION()
	void InitializeWidget(FName ObjectIDName, TSoftObjectPtr<UTexture2D> ThumpNail);
	
};
