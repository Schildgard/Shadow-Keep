// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementalType.h"
#include "NiagaraSystem.h"
#include "HitEffect.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FHitEffect : public FTableRowBase
{
	GENERATED_BODY()

public:
	FHitEffect();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EElementalType ElementalProperty;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//UNiagaraSystem* HitNiagaraEffect;
	


};
