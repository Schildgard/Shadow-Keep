// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "OpenChestTriggerNotify.generated.h"

/**
 * 
 */
class ATreasureChest;
UCLASS()
class ETERNALGRACE_PROTO_API UOpenChestTriggerNotify : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY()
	ATreasureChest* ChestToOpen;
	
};
