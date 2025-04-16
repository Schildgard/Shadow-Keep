// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSaveData.h"

FPlayerSaveData::FPlayerSaveData()
{

}

void FPlayerSaveData::UpdatePlayerData(FTransform NewTransform, FName Armor, FName Pants, FName Helmet, TMap<FName, int>* ArmorDataToSave, TMap<FName, int>* PantsDataToSave, TMap<FName, int>* HelmetDataToSave, TArray<TSubclassOf<AWeaponBase>>* WeaponMapToSave, TSubclassOf<AWeaponBase>*  CurrentWeaponToSave, TSubclassOf<AWeaponBase>* CurrentOffHandWeaponToSave, TMap<FName, int>* KeyItemsToSave)
{
	//Save Player Data
	PlayerTransform = NewTransform;
	CurrentBreastArmorName = Armor;
	CurrentPantsName = Pants;
	CurrentHelmetsName = Helmet;


	SavedArmorDataMap = *ArmorDataToSave;
	SavedPantsDataMap = *PantsDataToSave;
	SavedHelmetDataMap = *HelmetDataToSave;

	SavedWeaponInventory = *WeaponMapToSave;
	CurrentWeapon = *CurrentWeaponToSave;
	CurrentOffHandWeapon = *CurrentOffHandWeaponToSave;

	SavedKeyItemDataMap = *KeyItemsToSave;
}

