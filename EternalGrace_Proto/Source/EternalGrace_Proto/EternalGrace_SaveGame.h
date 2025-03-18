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
class AEternalGrace_ProtoCharacter;
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


	//General Game Settings Data
	UPROPERTY()
	bool bIsTwoPlayerModeActivated;
	UPROPERTY()
	TMap<int, TSubclassOf<AEternalGrace_ProtoCharacter>> PlayerClassMap;


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

	UFUNCTION()
	void SetPlayerMap(TMap<int, TSubclassOf<AEternalGrace_ProtoCharacter>> PlayerMap);
	UFUNCTION()
	TMap<int, TSubclassOf<AEternalGrace_ProtoCharacter>> GetPlayerMap();


	UFUNCTION()
	void SavePlayerData(FName ObjectID, FPlayerSaveData NewSaveData);

	
	FPlayerSaveData* LoadPlayerData(FName ObjectID);
	
	FTreasureChestSaveData* LoadTreasureChestData(FName ObjectID);

	UFUNCTION()
	void SaveTreasureChestData(FName ObjectID, FTreasureChestSaveData NewSaveData);

	
};
