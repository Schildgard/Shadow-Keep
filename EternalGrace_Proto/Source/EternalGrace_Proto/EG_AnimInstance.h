// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponType.h"
#include "ActionState.h"
#include "CharacterAnimInstanceBase.h"
#include "EG_AnimInstance.generated.h"

/**
 * 
 */
class AEternalGrace_ProtoCharacter;
UCLASS()
class ETERNALGRACE_PROTO_API UEG_AnimInstance : public UCharacterAnimInstanceBase
{
	GENERATED_BODY()

protected:
	UEG_AnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta =(AllowPrivateAccess))
	EWeaponType WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess))
	AEternalGrace_ProtoCharacter* OwningCharacter;

	void NativeBeginPlay()override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States", meta = (AllowPrivateAccess))
	EActionState CharacterActionState; //Inherited Class also has "Current Action State" - remove that one!
	UFUNCTION()
	void SetWeaponType(EWeaponType NewWeaponClass);
	UFUNCTION()
	void SetActionState(EActionState NewActionState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta =(AllowPrivateAccess))
	int AttackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess))
	bool bIsLockedOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess))
	bool bCanContinueAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess))
	bool bCanOffhandAttack;

	UFUNCTION(BlueprintCallable)
	void ResetAttackState();
	
};
