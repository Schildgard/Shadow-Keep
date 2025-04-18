// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/CapsuleComponent.h"
#include "WeaponBase.h"
#include "ShieldBase.h"
#include "Attackable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ETERNALGRACE_PROTO_API IAttackable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UCapsuleComponent* GetHitBox();
	//Get Weapon is Deprecated / not used !
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AWeaponBase* GetWeapon();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AWeaponBase* GetOffhandWeapon();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AShieldBase* GetShield();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Attack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<TEnumAsByte<EObjectTypeQuery>> GetHittableObjectTypes();
};
