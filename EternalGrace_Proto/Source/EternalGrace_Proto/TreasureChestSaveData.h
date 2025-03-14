// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreasureChestSaveData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTreasureChestSaveData : public FTableRowBase
{
	GENERATED_BODY()
public:

	FTreasureChestSaveData() :bCanBeInteractedWith(true) {};

	UPROPERTY()
	bool bCanBeInteractedWith;

};
