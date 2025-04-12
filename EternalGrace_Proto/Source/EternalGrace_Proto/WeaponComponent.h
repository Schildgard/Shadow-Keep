// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponType.h"
#include "WeaponComponent.generated.h"


class AWeaponBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETERNALGRACE_PROTO_API UWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TSubclassOf<AWeaponBase> CurrentWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<AWeaponBase> OffhandWeaponClass;

	UPROPERTY()
	AWeaponBase* CurrentWeaponObject;
	UPROPERTY()
	AWeaponBase* OffhandWeaponObject;

	UFUNCTION()
	EWeaponType ChangeWeapon(TSubclassOf<AWeaponBase> WeaponToEquip);
	UFUNCTION()
	void ChangeOffhandWeapon(TSubclassOf<AWeaponBase> WeaponToEquip);

	UPROPERTY()
	TArray<UAnimMontage*> NormalWeaponAttacks;

	UPROPERTY()
	UAnimMontage* RunningAttack;

	UPROPERTY()
	UAnimMontage* OffhandAttack;

	UPROPERTY()
	UAnimMontage* DodgeAttack;

	UPROPERTY()
	USoundBase* BlockSound;

};
