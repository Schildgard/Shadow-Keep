// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	NoWeapon UMETA(DisplayName = "NoWeapon"),
	StraightSword UMETA(DisplayName = "StraightSword"),
	GreatSword UMETA(DisplayName = "GreatSword")
};
