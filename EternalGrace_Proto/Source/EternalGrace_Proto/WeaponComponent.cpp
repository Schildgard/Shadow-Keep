// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "WeaponBase.h"
#include "EG_AnimInstance.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	CurrentWeaponClass = nullptr;
	CurrentWeaponObject = nullptr;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
//	if (CurrentWeaponClass)
//	{
//		ChangeWeapon(CurrentWeaponClass);
//
//		//	CurrentWeaponObject = GetWorld()->SpawnActor<AWeaponBase>(CurrentWeaponClass);
//		//	if (!CurrentWeaponObject)
//		//	{
//		//		UE_LOG(LogTemp, Error, TEXT("Failed to Spawn Weapon (WeaponComponent"));
//		//		return;
//		//	}
//		//	CurrentWeaponObject->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
//
//
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("No SubClassOfWeapon Assigned (WeaponComponent"));
//	}


}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

EWeaponType UWeaponComponent::ChangeWeapon(TSubclassOf<AWeaponBase> WeaponToEquip)
{
	if (WeaponToEquip)
	{
		//CurrentWeaponObject->Destroy();
		CurrentWeaponClass = WeaponToEquip;
		CurrentWeaponObject = GetWorld()->SpawnActor<AWeaponBase>(CurrentWeaponClass);
		if (!CurrentWeaponObject)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Spawn Weapon (WeaponComponent"));
			return EWeaponType::NoWeapon;
		}
		CurrentWeaponObject->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		UE_LOG(LogTemp, Error, TEXT("Attached. Now return Category (WeaponComponent"));
		return CurrentWeaponObject->WeaponCategory;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No SubClassOfWeapon Assigned (WeaponComponent"));
		return EWeaponType::NoWeapon;
	}
}

