// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterClassSelectWidget.generated.h"

/**
 *
 */
class UButtonWidget;
class UEternalGrace_GameInstance;
UCLASS()
class ETERNALGRACE_PROTO_API UCharacterClassSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct()override;
	UPROPERTY(meta = (BindWidget), Transient)
	UButtonWidget* Class1Button;
	UPROPERTY(meta = (BindWidget), Transient)
	UButtonWidget* Class2Button;

	UPROPERTY()
	UEternalGrace_GameInstance* CurrentGameInstance;

	UFUNCTION()
	void SetClass1();
	UFUNCTION()
	void SetClass2();

};
