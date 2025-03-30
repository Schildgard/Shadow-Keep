// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionState.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EActionState : uint8
{
	Idle UMETA(DisplayName = "IdleState"),
	Attacking UMETA(DisplayName = "AttackState"),
	Running UMETA(DisplayName = "RunningState"),
	Guarding UMETA(DisplayName = "GuardState"),
	Jumping UMETA(DisplayName = "JumpState"),
	Dodging UMETA(DisplayName = "DodgeState")
};
