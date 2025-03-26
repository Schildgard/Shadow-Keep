// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementalType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class  EElementalType :uint8
{
	Physical UMETA(DisplayName = "PhysicalDamage"),
	Fire UMETA(DisplayName = "FireDamage"),
	Holy UMETA(DisplayName = "HolyDamage"),
	Dark UMETA(DisplayName = "Dark")
};
