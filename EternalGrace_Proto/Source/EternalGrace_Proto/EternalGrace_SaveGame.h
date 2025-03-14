// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSaveData.h"
#include "TreasureChestSaveData.h"
#include "GameFramework/SaveGame.h"
#include "EternalGrace_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALGRACE_PROTO_API UEternalGrace_SaveGame : public USaveGame
{
	GENERATED_BODY()

protected:
	UEternalGrace_SaveGame();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Groups", meta = (AllowPrivateAccess))
	TMap<FName, FPlayerSaveData> PlayerSaveDataMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Groups", meta = (AllowPrivateAccess))
	TMap<FName, FTreasureChestSaveData> TreasureChestSaveDataMap;

	UPROPERTY()
	bool bIsTwoPlayerModeActivated;


public:

	UFUNCTION()
	void SetTwoPlayerModeInfo(bool ActivateInfo);
	UFUNCTION()
	bool GetTwoPlayerModeInfo();
	UPROPERTY()
	FString SlotID;

	UFUNCTION()
	void SaveAllData(TArray<AActor*> NewSaveableActorArray);
	UFUNCTION()
	void LoadAllData(TArray<AActor*> NewSaveableActorArray);

	UFUNCTION()
	void SaveTheGame();

	UFUNCTION()
	bool CheckSaveDataMap(FName ObjectID, FPlayerSaveData PlayerData);
	UFUNCTION()
	bool CheckTreasureSaveDataMap(FName ObjectID, FTreasureChestSaveData TreasureData);

	//UFUNCTION()
	//TMap<FName, FTreasureChestSaveData>& GetTreasureChestSaveDataMap();
	//
	//UFUNCTION()
	//TMap<FName, FPlayerSaveData>& GetPlayerSaveDataMap();

	UFUNCTION()
	void SavePlayerData(FName ObjectID, FPlayerSaveData NewSaveData);

	
	FPlayerSaveData* LoadPlayerData(FName ObjectID);
	
	FTreasureChestSaveData* LoadTreasureChestData(FName ObjectID);

	UFUNCTION()
	void SaveTreasureChestData(FName ObjectID, FTreasureChestSaveData NewSaveData);

	
};
