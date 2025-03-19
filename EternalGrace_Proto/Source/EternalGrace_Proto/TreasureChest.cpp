// Fill out your copyright notice in the Description page of Project Settings.


#include "TreasureChest.h"
#include "EternalGrace_ProtoCharacter.h"
#include "InventoryComponent.h"
#include "EternalGrace_GameInstance.h"
#include "EternalGrace_SaveGame.h"

void ATreasureChest::GetAllTreasure()
{
	if (!Interactor)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Interactor assigned to this chest! %s"), *GetFName().ToString());
		return;
	}

	if (!SaveDataInfo.bCanBeInteractedWith)
	{
		UE_LOG(LogTemp, Warning, TEXT("This Chest %s has already been opened"), *GetFName().ToString())
			return;
	}

	// Interactor checks in Armor Component if this Armor generally exists.
	// If so, The Inventory Component checks if its already in player inventory and adds it accordingly.
	for (FName ArmorName : ContainedArmory)
	{
		Interactor->ObtainArmor(ArmorName, EObjectType::BodyEquipment);
	}
	ContainedArmory.Empty();
	for (FName PantsName : ContainedLegArmor)
	{
		Interactor->ObtainArmor(PantsName, EObjectType::LegEquipment);
	}
	ContainedLegArmor.Empty();
	for (FName HelmetName : ContainedHelmets)
	{
		Interactor->ObtainArmor(HelmetName, EObjectType::HeadEquipment);
	}

	ContainedHelmets.Empty();
	bCanbeActivated = false;
	Execute_SaveData(this);
	Execute_SaveData(Interactor);

	Interactor = nullptr;
}

void ATreasureChest::GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)
{
	Interactor = InteractingPlayer;
	Super::GetInteractedWith_Implementation(InteractingPlayer);
}

void ATreasureChest::SaveData_Implementation()
{
	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetGameInstance());
	if (CurrentGameInstance)
	{
		UEternalGrace_SaveGame* CurrentSavegame = CurrentGameInstance->GetCurrentSaveGame();
		if (CurrentSavegame)
		{
			//Update SaveData Struct
			SaveDataInfo.bCanBeInteractedWith = bCanbeActivated;
			//Actually SaveData to SaveGame
			CurrentSavegame->SaveTreasureChestData(ObjectID, SaveDataInfo);
		}
	}
}

void ATreasureChest::LoadData_Implementation()
{
	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetGameInstance());
	if (CurrentGameInstance)
	{
		UEternalGrace_SaveGame* CurrentSavegame = CurrentGameInstance->GetCurrentSaveGame();
		{
			if (CurrentSavegame)
			{
				FTreasureChestSaveData* SavedData = CurrentSavegame->LoadTreasureChestData(ObjectID);
				//Try to load Data
				if (SavedData)
				{
					SaveDataInfo = *SavedData;
						if (!SaveDataInfo.bCanBeInteractedWith)
						{
							//When Chest has already been opened, trigger open anim
							Mesh->Play(false);
						}
				}
			}
		}
	}
}
