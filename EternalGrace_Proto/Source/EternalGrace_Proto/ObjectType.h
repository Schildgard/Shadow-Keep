// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ObjectType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EObjectType : uint8
{
	BodyEquipment	UMETA(DisplayName = "Breastplate"),
	HeadEquipment	UMETA(DisplayName = "Helmet"),
	LegEquipment	UMETA (DisplayName ="Greaves"),
	ArmEquipment	UMETA(DisplayName = "Gauntlets"),
	UseableItem		UMETA(DisplayName = "UseableObject"),
	KeyItem			UMETA(DisplayName = "KeyObject"),
	Weapon			UMETA(DisplayName = "Weapon"),
};
