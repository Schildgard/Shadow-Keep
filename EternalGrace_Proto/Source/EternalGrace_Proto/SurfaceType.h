// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "SurfaceType.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  FSurfaceType : public FTableRowBase
{
	GENERATED_BODY()
public:
	FSurfaceType();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta =(AllowPrivateAccess))
	USoundBase* FootStepSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	USoundBase* WeaponHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UNiagaraSystem* FootStepEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UNiagaraSystem* WeaponHitEffect;
};
