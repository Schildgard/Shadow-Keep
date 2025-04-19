// Fill out your copyright notice in the Description page of Project Settings.


#include "ObtainableActor.h"
#include "EternalGrace_ProtoCharacter.h"
#include "InventoryComponent.h"
#include "EternalGrace_GameInstance.h"
#include "EternalGrace_SaveGame.h"
#include "Components/SphereComponent.h"


AObtainableActor::AObtainableActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("SkeletalMesh");
	SetRootComponent(Mesh);
	InteractionSphere->SetupAttachment(RootComponent);
	Interactor = nullptr;
}

void AObtainableActor::PickUp()
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

	for (TSubclassOf<AWeaponBase> Weapon : ContainedWeapons)
	{
		Interactor->GetInventory()->AddWeaponToInventory(Weapon);
	}

	for (FName KeyItemName : ContainedKeyItems)
	{
		Interactor->GetInventory()->TryToObtainItem(KeyItemName, EObjectType::KeyItem);
	}
	ContainedKeyItems.Empty();


	ContainedWeapons.Empty();
	Execute_SaveData(this);

	Interactor = nullptr;
	Destroy();
}

void AObtainableActor::GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)
{
	if (!bCanbeActivated)
	{
		UE_LOG(LogTemp, Error, TEXT("Trease can not be Activated anymore"));
		return;
	}
	bCanbeActivated = false;
	Interactor = InteractingPlayer;
	PickUp();
}

void AObtainableActor::SaveData_Implementation()
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
		CurrentGameInstance->SavePlayerData();
	}
}

void AObtainableActor::LoadData_Implementation()
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
						//When Item already has been picked up, destroy it immedeately
						Destroy();
					}
				}
			}
		}
	}

}
