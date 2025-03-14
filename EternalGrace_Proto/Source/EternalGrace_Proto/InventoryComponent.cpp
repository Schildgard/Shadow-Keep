#include "InventoryComponent.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Armor.h"
#include "Pants.h"
#include "Helmet.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	GlobalArmorInventory = nullptr;
	GlobalPantsInventory = nullptr;
	GlobalHelmetInventory = nullptr;

	ArmorInventory = nullptr;
	PantsInventory = nullptr;
	HelmetInventory = nullptr;
}



void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	ArmorInventory = NewObject<UDataTable>(this);
	if (ArmorInventory)
	{
			ArmorInventory->RowStruct = FArmor::StaticStruct();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Create ArmorInventory Table at Start of the Game"))
	}

	PantsInventory = NewObject<UDataTable>(this);
	if (PantsInventory)
	{
			PantsInventory->RowStruct = FPants::StaticStruct();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Create PantsInventory Table at Start of the Game"))
	}

	HelmetInventory = NewObject<UDataTable>(this);
	if (HelmetInventory)
	{
			HelmetInventory->RowStruct = FHelmet::StaticStruct();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Create HelmetInventory Table at Start of the Game"))
	}
}

UDataTable* UInventoryComponent::GetArmorInventory()
{
	return ArmorInventory;
}
UDataTable* UInventoryComponent::GetPantsInventory()
{
	return PantsInventory;
}
UDataTable* UInventoryComponent::GetHelmetInventory()
{
	return HelmetInventory;
}

void UInventoryComponent::AddArmorToInventory(FArmor ArmorToAdd)
{
	//This Function decides if the player can put the existing Item in his inventory and what happens if he already owns it.
	FName ArmorNameToFind = ArmorToAdd.ArmorName;
	static const FString ContextString(TEXT("NewArmorContext"));
	FArmor* FoundArmor = ArmorInventory->FindRow<FArmor>(ArmorNameToFind, ContextString, false);
	if (FoundArmor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Already owns this Object"))
			return;
	}
	ArmorInventory->AddRow(ArmorNameToFind, ArmorToAdd);
}

void UInventoryComponent::AddPantsToInventory(FPants PantsToAdd)
{
	//This Function decides if the player can put the existing Item in his inventory and what happens if he already owns it.
	FName PantsNameToFind = PantsToAdd.PantsName;
	static const FString ContextString(TEXT("NewPantsContext"));
	FPants* FoundArmor = PantsInventory->FindRow<FPants>(PantsNameToFind, ContextString, false);
	if (FoundArmor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Already owns this Object"))
			return;
	}
	PantsInventory->AddRow(PantsNameToFind, PantsToAdd);
}

void UInventoryComponent::AddHelmetToInventory(FHelmet HelmetToAdd)
{
	//This Function decides if the player can put the existing Item in his inventory and what happens if he already owns it.
	FName HelmetsNameToFind = HelmetToAdd.HelmetName;
	static const FString ContextString(TEXT("NewArmorContext"));
	FHelmet* FoundArmor = HelmetInventory->FindRow<FHelmet>(HelmetsNameToFind, ContextString, false);
	if (FoundArmor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Already owns this Object"))
			return;
	}
	HelmetInventory->AddRow(HelmetsNameToFind, HelmetToAdd);
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

