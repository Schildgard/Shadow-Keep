// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "WeaponBase.h"
#include "EG_AnimInstance.h"
#include "CharacterBase.h"


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentWeaponClass = nullptr;
	CurrentWeaponObject = nullptr;
	RunningAttack = nullptr;
}

EWeaponType UWeaponComponent::ChangeWeapon(TSubclassOf<AWeaponBase> WeaponToEquip)
{
	if (WeaponToEquip)
	{
		ACharacterBase* OwningCharacter = Cast<ACharacterBase>(GetOwner());
		if (!OwningCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Cast Character (WeaponComponent"));
			return EWeaponType::NoWeapon;
		}
		if(CurrentWeaponObject)
		{
			CurrentWeaponObject->Destroy();
			NormalWeaponAttacks.Empty();
			RunningAttack = nullptr;
			DodgeAttack = nullptr;
		}
		CurrentWeaponClass = WeaponToEquip;
		CurrentWeaponObject = GetWorld()->SpawnActor<AWeaponBase>(CurrentWeaponClass);
		if (!CurrentWeaponObject)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Spawn Weapon (WeaponComponent"));
			return EWeaponType::NoWeapon;
		}
		CurrentWeaponObject->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "s_hand_r");
		NormalWeaponAttacks = CurrentWeaponObject->GetNormalAttacks();
		RunningAttack = CurrentWeaponObject->GetRunningAttack();
		DodgeAttack = CurrentWeaponObject->GetDodgeAttack();
		/*If Weapon is TwoHanded, Unequip Offhand Weapon*/
		if (WeaponToEquip->GetDefaultObject<AWeaponBase>()->WeaponCategory == EWeaponType::GreatSword && OffhandWeaponObject)
		{
			OffhandWeaponObject->Destroy();
			OffhandAttack = nullptr;
		}

		return CurrentWeaponObject->WeaponCategory;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No SubClassOfWeapon Assigned (WeaponComponent"));
		return EWeaponType::NoWeapon;
	}
}

void UWeaponComponent::ChangeOffhandWeapon(TSubclassOf<AWeaponBase> WeaponToEquip)
{
	if (WeaponToEquip)
	{
		ACharacterBase* OwningCharacter = Cast<ACharacterBase>(GetOwner());
		if (!OwningCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Cast Character (WeaponComponent"));
			return;
		}

		if(OffhandWeaponObject)
		{
		OffhandWeaponObject->Destroy();
		OffhandAttack = nullptr;
		}
		OffhandWeaponClass = WeaponToEquip;
		OffhandWeaponObject = GetWorld()->SpawnActor<AWeaponBase>(OffhandWeaponClass);
		if (!OffhandWeaponObject)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Spawn Weapon (WeaponComponent"));
			return;
		}
		OffhandWeaponObject->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "s_hand_l");
		OffhandAttack = OffhandWeaponObject->GetOffhandAttack();
	}

}

