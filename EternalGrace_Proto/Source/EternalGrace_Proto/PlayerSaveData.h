// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Armor.h"
#include "Pants.h"
#include "Helmet.h"
#include "WeaponBase.h"
#include "PlayerSaveData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlayerSaveData : public FTableRowBase
{
	GENERATED_BODY()
	FPlayerSaveData();
public:

	UPROPERTY()
	FTransform PlayerTransform;

	UPROPERTY()
	FName CurrentBreastArmorName;
	UPROPERTY()
	FName CurrentPantsName;
	UPROPERTY()
	FName CurrentHelmetsName;

	UPROPERTY()
	TSubclassOf<AWeaponBase> CurrentWeapon;
	UPROPERTY()
	TSubclassOf<AWeaponBase> CurrentOffHandWeapon;


	UPROPERTY()
	TMap<FName,int> SavedArmorDataMap;
	UPROPERTY()
	TMap<FName,int> SavedPantsDataMap;
	UPROPERTY()
	TMap<FName,int> SavedHelmetDataMap;

	UPROPERTY()
	TArray<TSubclassOf<AWeaponBase>> SavedWeaponInventory;

	void UpdatePlayerData(FTransform NewTransform, FName Armor, FName Pants, FName Helmet, TMap<FName, int>* ArmorDataToSave, TMap<FName, int>* PantsDataToSave, TMap<FName, int>* HelmetDataToSave, TArray<TSubclassOf<AWeaponBase>>* WeaponToSave, TSubclassOf<AWeaponBase>* CurrentWeaponToSave, TSubclassOf<AWeaponBase>* CurrentOffHandWeaponToSave);
};
