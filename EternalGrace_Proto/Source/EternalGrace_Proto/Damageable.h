// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/AudioComponent.h"
#include "NiagaraSystem.h"
#include "AttackDirection.h"
#include "Damageable.generated.h"

UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ETERNALGRACE_PROTO_API IDamageable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void GetDamage(AActor* Attacker, float DamageValue, float PoiseDamage, EAttackDirection AttackDirection, FVector HitLocation, FRotator HitRotation);

	UFUNCTION(BlueprintNativeEvent)
	UAudioComponent* GetHitSoundComponent();

	UFUNCTION(BlueprintNativeEvent)
	UNiagaraSystem* GetHitEffectSystem();

	UFUNCTION(BlueprintNativeEvent)
	void Die();
};
