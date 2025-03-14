// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

/**
 * 
 */
class UCanvasPanel;
class UImage;
class UWrapBox;
UCLASS()
class ETERNALGRACE_PROTO_API UInventory : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct()override;
	UPROPERTY(meta =(BindWidget))
	UCanvasPanel* InventoryCanvas;

	UPROPERTY(meta = (BindWidget))
	UImage* InventoryBackground;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryWrapBox;
public:
	UFUNCTION()
	UWrapBox* GetWrapBox();
	
};
