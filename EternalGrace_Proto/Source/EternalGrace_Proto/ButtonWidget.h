// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonWidget.generated.h"

/**
 * 
 */
class UButton;
UCLASS()
class ETERNALGRACE_PROTO_API UButtonWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct()override;
	void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent);
	void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(BindWidget, AllowPrivateAccess), Transient)
	UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="User Feedback", meta =(AllowPrivateAccess))
	USoundBase* SelectionSound;
public:
	UFUNCTION()
	UButton* GetWidgetButton();
	UFUNCTION()
	void RegisterSelection();
	
};
