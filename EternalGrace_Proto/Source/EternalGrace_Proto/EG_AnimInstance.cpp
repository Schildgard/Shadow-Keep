// Fill out your copyright notice in the Description page of Project Settings.


#include "EG_AnimInstance.h"

UEG_AnimInstance::UEG_AnimInstance()
{
	WeaponClass = EWeaponType::NoWeapon;
}

void UEG_AnimInstance::SetWeaponType(EWeaponType NewWeaponClass)
{
	WeaponClass = NewWeaponClass;
}
