// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponType.h"
#include "EG_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALGRACE_PROTO_API UEG_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta =(AllowPrivateAccess))
	EWeaponType WeaponClass;

	UEG_AnimInstance();
public:
	UFUNCTION()
	void SetWeaponType(EWeaponType NewWeaponClass);
	
};
