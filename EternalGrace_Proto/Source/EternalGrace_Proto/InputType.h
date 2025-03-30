// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EInputType :uint8
{
	NoInput UMETA(DisplayName = "NoInput"),
	NormalAttack UMETA(DisplayName = "NormalAttackAction"),
	HeavyAttack UMETA(DisplayName = "HeavyAttackAction"),
	DodgeAttack UMETA(DisplayName = "DodgeAttackAction"),
	OffhandAttack UMETA(DisplayName = "OffhandAttackAction"),
	Dodge UMETA(DisplayName = "DodgeAction"),
	Interact UMETA(DisplayName = "InteractAction"),
	Jump UMETA(DisplayName = "JumPAction")
};
