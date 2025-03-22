// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Armor.h"
#include "Pants.h"
#include "Helmet.h"
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
	TArray<FArmor> SavedArmorData;
	UPROPERTY()
	TArray<FPants> SavedPantsData;
	UPROPERTY()
	TArray<FHelmet> SavedHelmetData;


	UPROPERTY()
	TMap<FName,int> SavedArmorDataMap;
	UPROPERTY()
	TMap<FName,int> SavedPantsDataMap;
	UPROPERTY()
	TMap<FName,int> SavedHelmetDataMap;

	void SetPlayerTransform(FTransform NewTransform, FName Armor, FName Pants, FName Helmet, UDataTable* ArmorDataToSave, UDataTable* PantsDataToSave, UDataTable* HelmetDataToSave);
	void UpdatePlayerData(FTransform NewTransform, FName Armor, FName Pants, FName Helmet, TMap<FName, int>* ArmorDataToSave, TMap<FName, int>* PantsDataToSave, TMap<FName, int>* HelmetDataToSave);
};
