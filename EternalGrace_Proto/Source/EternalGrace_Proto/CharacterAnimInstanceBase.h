// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActionState.h"
#include "CharacterAnimInstanceBase.generated.h"

/**
 * 
 */
class UCharacterMovementComponent;
UCLASS()
class ETERNALGRACE_PROTO_API UCharacterAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
protected:
	void NativeInitializeAnimation()override;
	void NativeUpdateAnimation(float DeltaSeconds)override;

	UPROPERTY()
	ACharacter* Character;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess))
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess))
	float FacingDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess))
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Movement", meta = (AllowPrivateAccess))
	bool bIsFalling;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess))
	EActionState CurrentActionState;
	
};
