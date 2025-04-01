// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemObtainContainerWidget.generated.h"

/**
 * 
 */
class UStackBox;
class UButton;
UCLASS()
class ETERNALGRACE_PROTO_API UItemObtainContainerWidget : public UUserWidget
{
	GENERATED_BODY()

	void NativeConstruct()override;
public:
	UPROPERTY(meta =(BindWidget, Transient))
	UStackBox* StackBox;

	UPROPERTY(meta = (BindWidget), Transient)
	UButton* RemoveWidgetButton;

	UFUNCTION()
	void RemoveWidget();

};
