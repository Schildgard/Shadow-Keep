// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "ArmorComponent.h"
#include "EG_PlayerController.h"

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	OwningCharacter = nullptr;
	ItemCategory = EObjectType::KeyItem;
}

void UInventorySlot::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	RegisterSelection();
}

void UInventorySlot::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
}

void UInventorySlot::UseInventorySlot()
{

	if (!OwningCharacter)
	{
		AEG_PlayerController* EGController = Cast<AEG_PlayerController>(GetOwningPlayer());
		if (EGController)
		{
			OwningCharacter = EGController->GetOwningCharacter();
			if (!OwningCharacter)
			{
				UE_LOG(LogTemp, Error, TEXT("Controller Huan not get"))
				return;
			}
		}
	}

	switch (ItemCategory)
	{
	case EObjectType::BodyEquipment:
		OwningCharacter->ChangeUpperArmor(ItemName);
		break;
	case EObjectType::HeadEquipment:
		OwningCharacter->ChangeHelmet(ItemName);
		break;
	case EObjectType::LegEquipment:
		OwningCharacter->ChangePants(ItemName);
		break;
	case EObjectType::ArmEquipment:
		UE_LOG(LogTemp, Warning, TEXT("There is no use Function for Arms Equipment implemented yet. Check InventorySlotClass"))
			break;
	case EObjectType::UseableItem:
		UE_LOG(LogTemp, Warning, TEXT("There is no use Function for this object implemented yet. Check InventorySlotClass"))
			break;
	case EObjectType::KeyItem:
		UE_LOG(LogTemp, Warning, TEXT("KeyItems are not to be used. Check InventorySlotClass"))
			break;
	case EObjectType::Weapon:
		OwningCharacter->EquipWeapon(WeaponClass);
	default:
		UE_LOG(LogTemp, Warning, TEXT("No Valid ObjectType. Check InventorySlotClass"))
			break;
	}


}

UButton* UInventorySlot::GetSlotButton()
{
	return SlotButton;
}

UImage* UInventorySlot::GetImage()
{
	return ItemImage;
}

void UInventorySlot::SetObjectType(EObjectType NewItemCategory)
{
	ItemCategory = NewItemCategory;
}

void UInventorySlot::SetImage(UTexture2D* ImageToSet)
{

	ItemImage->SetBrushFromTexture(ImageToSet);
}

void UInventorySlot::SetItemName(FName Name)
{
	ItemName = Name;
}

void UInventorySlot::SetWeapon(TSubclassOf<AWeaponBase> WeaponToRepresent)
{
	WeaponClass = WeaponToRepresent;
}

void UInventorySlot::RegisterSelection()
{
	PlaySound(SelectionSound);
}
