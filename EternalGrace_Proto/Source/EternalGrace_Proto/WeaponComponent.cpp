// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "WeaponBase.h"
#include "EG_AnimInstance.h"


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentWeaponClass = nullptr;
	CurrentWeaponObject = nullptr;
	RunningAttack = nullptr;
}

EWeaponType UWeaponComponent::ChangeWeapon(TSubclassOf<AWeaponBase> WeaponToEquip)
{
	if (WeaponToEquip)
	{
		//CurrentWeaponObject->Destroy();
		CurrentWeaponClass = WeaponToEquip;
		CurrentWeaponObject = GetWorld()->SpawnActor<AWeaponBase>(CurrentWeaponClass);
		if (!CurrentWeaponObject)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Spawn Weapon (WeaponComponent"));
			return EWeaponType::NoWeapon;
		}
		CurrentWeaponObject->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		NormalWeaponAttacks = CurrentWeaponObject->GetNormalAttacks();

		return CurrentWeaponObject->WeaponCategory;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No SubClassOfWeapon Assigned (WeaponComponent"));
		return EWeaponType::NoWeapon;
	}
}

