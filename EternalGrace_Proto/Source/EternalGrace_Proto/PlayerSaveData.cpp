// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSaveData.h"

FPlayerSaveData::FPlayerSaveData()
{

}

void FPlayerSaveData::SetPlayerTransform(FTransform NewTransform, FName Armor, FName Pants, FName Helmet, UDataTable* ArmorDataToSave, UDataTable* PantsDataToSave, UDataTable* HelmetDataToSave)
{
	//Save Player Data
	PlayerTransform = NewTransform;
	CurrentBreastArmorName = Armor;
	CurrentPantsName = Pants;
	CurrentHelmetsName = Helmet;

	//Save Armor Data
	static const FString Context(TEXT("SaveArmorContext"));
	TArray<FArmor*> IncomingArmorData;
	ArmorDataToSave->GetAllRows(Context, IncomingArmorData);
	for(FArmor* ArmorPointer : IncomingArmorData)
	{
		SavedArmorData.Add(*ArmorPointer);
		UE_LOG(LogTemp, Display, TEXT("Saved %s to SaveFile"), *ArmorPointer->ArmorName.ToString())
	}

	//Save Pants Data
	static const FString PantsContext(TEXT("SavePantsContext"));
	TArray<FPants*> IncomingPantsData;
	PantsDataToSave->GetAllRows(PantsContext, IncomingPantsData);
	for (FPants* PantsPointer : IncomingPantsData)
	{
		SavedPantsData.Add(*PantsPointer);
		UE_LOG(LogTemp, Display, TEXT("Saved %s to SaveFile"), *PantsPointer->PantsName.ToString())
	}

	//Save Helmet Data
	static const FString HelmetContext(TEXT("SaveHelmetContext"));
	TArray<FHelmet*> IncomingHelmetData;
	HelmetDataToSave->GetAllRows(HelmetContext, IncomingHelmetData);
	for (FHelmet* HelmetPointer : IncomingHelmetData)
	{
		SavedHelmetData.Add(*HelmetPointer);
		UE_LOG(LogTemp, Display, TEXT("Saved %s to SaveFile"), *HelmetPointer->HelmetName.ToString())
	}
}

