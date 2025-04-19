// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerableSaveData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  FTriggerableSaveData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FTriggerableSaveData() : bHasBeenTriggered(false) {};

	UPROPERTY()
	bool bHasBeenTriggered;
};
