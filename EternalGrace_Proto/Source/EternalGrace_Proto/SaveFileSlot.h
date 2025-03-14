// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveFileSlot.generated.h"

/**
 * 
 */
class UButton;
class UTextBlock;
UCLASS()
class ETERNALGRACE_PROTO_API USaveFileSlot : public UUserWidget
{
	GENERATED_BODY()

protected:

	void NativeConstruct();
	UPROPERTY(meta= (BindWidget), Transient)
	UButton* LoadFileButton;
	UPROPERTY(meta = (BindWidget), Transient)
	UButton* DeleteFileButton;
	UPROPERTY(meta = (BindWidget), Transient)
	UTextBlock* ButtonText;
	UPROPERTY()
	FName SlotID;
public:

	UFUNCTION()
	UButton* GetLoadButton();
	UFUNCTION()
	void SetSlotID(FName FileSlotID);
	UFUNCTION()
	void LoadGame();
	UFUNCTION()
	void DeleteFile();
	
};
