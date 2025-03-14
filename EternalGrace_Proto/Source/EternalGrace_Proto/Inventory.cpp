// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"

void UInventory::NativeConstruct()
{
}

UWrapBox* UInventory::GetWrapBox()
{
	return InventoryWrapBox;
}
