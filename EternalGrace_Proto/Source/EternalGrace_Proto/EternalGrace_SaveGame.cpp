// Fill out your copyright notice in the Description page of Project Settings.


#include "EternalGrace_SaveGame.h"
#include "Saveable.h"
#include "Kismet/GameplayStatics.h"


UEternalGrace_SaveGame::UEternalGrace_SaveGame()
{

}

void UEternalGrace_SaveGame::SetTwoPlayerModeInfo(bool ActivateInfo)
{
	bIsTwoPlayerModeActivated = ActivateInfo;
}

bool UEternalGrace_SaveGame::GetTwoPlayerModeInfo()
{
	return bIsTwoPlayerModeActivated;
}

void UEternalGrace_SaveGame::SaveAllData(TArray<AActor*> SaveableActorArray)
{
	for (AActor* Saveable : SaveableActorArray)
	{
		ISaveable::Execute_SaveData(Saveable);
	}
	UGameplayStatics::SaveGameToSlot(this, SlotID, 0);
}

void UEternalGrace_SaveGame::LoadAllData(TArray<AActor*> SaveableActorArray)
{
	for (AActor* Saveable : SaveableActorArray)
	{
		ISaveable::Execute_LoadData(Saveable);
	}
}

void UEternalGrace_SaveGame::SaveTheGame()
{
	UGameplayStatics::SaveGameToSlot(this, SlotID, 0);
}

bool UEternalGrace_SaveGame::CheckSaveDataMap(FName ObjectID, FPlayerSaveData PlayerData)
{
	UE_LOG(LogTemp, Display, TEXT("Check SaveDataMap for Object ID %s"), *ObjectID.ToString())
		if (!PlayerSaveDataMap.Contains(ObjectID))
		{
			PlayerSaveDataMap.Add(ObjectID, PlayerData);
			if (!PlayerSaveDataMap.Contains(ObjectID))
			{
				UE_LOG(LogTemp, Error, TEXT("%s could not be saved!"), *ObjectID.ToString());
				return false;
			}
		}
	return true;
}

bool UEternalGrace_SaveGame::CheckTreasureSaveDataMap(FName ObjectID, FTreasureChestSaveData TreasureData)
{
	UE_LOG(LogTemp, Display, TEXT("Check SaveDataMap for Object ID %s"), *ObjectID.ToString())
		if (!TreasureChestSaveDataMap.Contains(ObjectID))
		{
			TreasureChestSaveDataMap.Add(ObjectID, TreasureData);
			if (!TreasureChestSaveDataMap.Contains(ObjectID))
			{
				UE_LOG(LogTemp, Error, TEXT("%s could not be saved!"), *ObjectID.ToString());
				return false;
			}
		}
	return true;
}

bool UEternalGrace_SaveGame::CheckNPCSaveDataMap(FName ObjectID, FNPCSaveDataInfoBase NPCData)
{
	UE_LOG(LogTemp, Display, TEXT("Check SaveDataMap for Object ID %s"), *ObjectID.ToString())
		if(!NPCSaveDataMap.Contains(ObjectID))
		{
			NPCSaveDataMap.Add(ObjectID, NPCData);
			if(!NPCSaveDataMap.Contains(ObjectID))
			{
				UE_LOG(LogTemp, Error, TEXT("%s could not be saved!"), *ObjectID.ToString());
				return false;
			}
		}
	return true;
}

void UEternalGrace_SaveGame::SetPlayerMap(TMap<int, TSubclassOf<AEternalGrace_ProtoCharacter>> IncomingPlayerMap)
{
	PlayerClassMap = IncomingPlayerMap;
	SaveTheGame();
}

TMap<int, TSubclassOf<AEternalGrace_ProtoCharacter>> UEternalGrace_SaveGame::GetPlayerMap()
{
	return PlayerClassMap;
}

//LoadData functions are calles by Saveable Objects which load their SaveData Info from this Data Map!
FPlayerSaveData* UEternalGrace_SaveGame::LoadPlayerData(FName ObjectID)
{
	FPlayerSaveData* LoadData = PlayerSaveDataMap.Find(ObjectID);
	if(LoadData)
	{
		return LoadData;
	}
	return nullptr;
}

FTreasureChestSaveData* UEternalGrace_SaveGame::LoadTreasureChestData(FName ObjectID)
{
	FTreasureChestSaveData* LoadData = TreasureChestSaveDataMap.Find(ObjectID);
	if(LoadData)
	{
		return LoadData;
	}
	return nullptr;
}

FNPCSaveDataInfoBase* UEternalGrace_SaveGame::LoadNPCData(FName ObjectID)
{
	FNPCSaveDataInfoBase* LoadData = NPCSaveDataMap.Find(ObjectID);
	if(LoadData)
	{
		return LoadData;
	}
	return nullptr;
}

void UEternalGrace_SaveGame::SavePlayerData(FName ObjectID, FPlayerSaveData NewSaveData)
{
	
	if (CheckSaveDataMap(ObjectID, NewSaveData))
	{
		PlayerSaveDataMap[ObjectID] = NewSaveData;
		SaveTheGame();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CheckPlayerSaveDataMap Failed"))
	}
}
void UEternalGrace_SaveGame::SaveTreasureChestData(FName ObjectID, FTreasureChestSaveData NewSaveData)
{
	if (CheckTreasureSaveDataMap(ObjectID, NewSaveData))
	{
		TreasureChestSaveDataMap[ObjectID] = NewSaveData;
		SaveTheGame();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CheckTreasureSaveDataMap Failed"))
	}

}

void UEternalGrace_SaveGame::SaveNPCData(FName ObjectID, FNPCSaveDataInfoBase NewSaveData)
{
	if(CheckNPCSaveDataMap(ObjectID, NewSaveData))
	{
		NPCSaveDataMap[ObjectID] = NewSaveData;
		SaveTheGame();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CheckNPCDataMap Failed(SaveGame Class)"))
	}
}

