// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCSaveDataInfoBase.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  FNPCSaveDataInfoBase :public FTableRowBase
{
	GENERATED_BODY()

public:
	FNPCSaveDataInfoBase();
	UPROPERTY()
	bool bIsAlive;

	void UpdateNPCSaveData(bool bIsAlive);

};
