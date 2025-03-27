#include "InventoryComponent.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Armor.h"
#include "Pants.h"
#include "Helmet.h"
#include "WeaponBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	GlobalArmorInventory = nullptr;
	GlobalPantsInventory = nullptr;
	GlobalHelmetInventory = nullptr;
}



void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

TMap<FName, int>* UInventoryComponent::GetInventoryOfType(EObjectType ItemCategory)
{
	switch (ItemCategory)
	{
	case EObjectType::BodyEquipment:
		return &ArmorInventoryMap;
	case EObjectType::LegEquipment:
		return &PantsInventoryMap;
	case EObjectType::HeadEquipment:
		return &HelmetInventoryMap;
	default:
		UE_LOG(LogTemp, Error, TEXT("No Valid Object Type"))
		return nullptr;
	}
}

void UInventoryComponent::AddArmorToInventory(FArmor ArmorToAdd)
{
//	if(!ArmorInventoryMap.Contains(ArmorToAdd.ArmorName))
//	{
//		ArmorInventoryMap.Add(ArmorToAdd.ArmorName, 1);
//		UE_LOG(LogTemp, Warning, TEXT("Added %s to Inventory"), *ArmorToAdd.ArmorName.ToString())
//	}
//	else
//	{
//		ArmorInventoryMap[ArmorToAdd.ArmorName] += 1;
//		UE_LOG(LogTemp, Warning, TEXT("Added 1 more object of %s to Inventory"), *ArmorToAdd.ArmorName.ToString())
//	}


}

void UInventoryComponent::AddPantsToInventory(FPants PantsToAdd)
{
	if (!PantsInventoryMap.Contains(PantsToAdd.PantsName))
	{
		PantsInventoryMap.Add(PantsToAdd.PantsName, 0);
		UE_LOG(LogTemp, Warning, TEXT("Added %s to Inventory"), *PantsToAdd.PantsName.ToString())
	}
	else
	{
		PantsInventoryMap[PantsToAdd.PantsName] += 1;
		UE_LOG(LogTemp, Warning, TEXT("Added 1 more object of %s to Inventory"), *PantsToAdd.PantsName.ToString())
	}
}

void UInventoryComponent::AddHelmetToInventory(FHelmet HelmetToAdd)
{
	if (!HelmetInventoryMap.Contains(HelmetToAdd.HelmetName))
	{
		HelmetInventoryMap.Add(HelmetToAdd.HelmetName, 0);
		UE_LOG(LogTemp, Warning, TEXT("Added %s to Inventory"), *HelmetToAdd.HelmetName.ToString())
	}
	else
	{
		HelmetInventoryMap[HelmetToAdd.HelmetName] += 1;
		UE_LOG(LogTemp, Warning, TEXT("Added 1 more object of %s to Inventory"), *HelmetToAdd.HelmetName.ToString())
	}
}

void UInventoryComponent::AddWeaponToInventory(TSubclassOf<AWeaponBase> WeaponToAdd)
{
	WeaponInventory.Add(WeaponToAdd);

	//UE_LOG(LogTemp, Display, TEXT("Added to Inventory %s"), *WeaponToAdd->GetWeaponName().ToString());
}

void UInventoryComponent::TryToObtainItem(FName ObjectName, EObjectType ItemCategory)
{
	//This Function checks if this Item actually exists in global data base befor Adding it to inventory
	static const FString ContextString(TEXT("ObtainedArmorContext"));
	switch (ItemCategory)
	{
	case EObjectType::BodyEquipment:
	{
		FArmor* ArmorContext = GlobalArmorInventory->FindRow<FArmor>(ObjectName, ContextString);
		if (ArmorContext)
		{
			AddArmorToInventory(*ArmorContext);
		}
		break;
	}
	case EObjectType::LegEquipment:
	{
		FPants* PantsContext = GlobalPantsInventory->FindRow<FPants>(ObjectName, ContextString);
		if (PantsContext)
		{
			AddPantsToInventory(*PantsContext);
		}
		break;
	}
	case EObjectType::HeadEquipment:
	{
		FHelmet* HelmetContext = GlobalHelmetInventory->FindRow<FHelmet>(ObjectName, ContextString);
		if (HelmetContext)
		{
			AddHelmetToInventory(*HelmetContext);
		}
		break;
	}
	default:
		UE_LOG(LogTemp, Error,TEXT("%s has no valid ItemCategory"), *ObjectName.ToString())
		return;
	}
}

