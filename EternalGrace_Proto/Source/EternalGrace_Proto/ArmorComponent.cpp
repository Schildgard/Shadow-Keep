// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorComponent.h"
#include "Armor.h"
#include "Pants.h"
#include "Helmet.h"

UArmorComponent::UArmorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	BreastPlateArmorTable = nullptr;

	Body = CreateDefaultSubobject<USkeletalMeshComponent>("BodyArmorMesh");
	Body->PrimaryComponentTick.bCanEverTick = false;
	Arms = CreateDefaultSubobject<USkeletalMeshComponent>("ArmsArmorMesh");
	Arms->PrimaryComponentTick.bCanEverTick = false;
	Shoulders = CreateDefaultSubobject<USkeletalMeshComponent>("ShoulderArmorMesh");
	Shoulders->PrimaryComponentTick.bCanEverTick = false;
	CurrentBreastplateName = "NoArmor";

	Pants = CreateDefaultSubobject<USkeletalMeshComponent>("PantsMesh");
	Pants->PrimaryComponentTick.bCanEverTick = false;
	CurrentPantsName = "NoPants";

	Helmet = CreateDefaultSubobject<USkeletalMeshComponent>("HelmetMesh");
	Helmet->PrimaryComponentTick.bCanEverTick = false;
	CurrentHelmetsName = "NoHelmet";

	ArmorSoundComponent = CreateDefaultSubobject<UAudioComponent>("Armor Sound");
}

UAudioComponent* UArmorComponent::GetArmorSoundComponent()
{
	return ArmorSoundComponent;
}

void UArmorComponent::EquipBreastPlate(FName ArmorName)
{
	CurrentBreastplateName = ArmorName;
	//Search general DataTable for ArmorData and Set Data
	static const FString ContextString(TEXT("NewArmorContext"));
	FArmor* ArmorData = BreastPlateArmorTable->FindRow<FArmor>(CurrentBreastplateName, ContextString);
	if (ArmorData)
	{
		Body->SetSkeletalMesh(ArmorData->BreastplateMesh.LoadSynchronous());
		Arms->SetSkeletalMesh(ArmorData->ArmsMesh.LoadSynchronous());
		Shoulders->SetSkeletalMesh(ArmorData->ShoulderMesh.LoadSynchronous());
		bIsWearingHeavyArmor = ArmorData->bIsHeavyArmor;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s was not found in BreastPlateArmorTable"), *ArmorName.ToString())
	}
}

void UArmorComponent::EquipPants(FName PantsName)
{

	CurrentPantsName = PantsName;
	//Search general DataTable for ArmorData and Set Data
	static const FString ContextString(TEXT("NewPantsContext"));
	FPants* PantsData = PantsTable->FindRow<FPants>(CurrentPantsName, ContextString);
	if (PantsData)
	{
		Pants->SetSkeletalMesh(PantsData->PantsMesh.LoadSynchronous());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s was not found in PantsDataTable"), *PantsName.ToString())
	}
}

void UArmorComponent::EquipHelmet(FName HelmetName)
{
	CurrentHelmetsName = HelmetName;
	static const FString ContextString(TEXT("NewHelmetContext"));
	FHelmet* HelmetData = HelmetTable->FindRow<FHelmet>(CurrentHelmetsName, ContextString);
	if (HelmetData)
	{
		Helmet->SetSkeletalMesh(HelmetData->HelmetMesh.LoadSynchronous());		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s was not found in HelmetDataTable"), *HelmetName.ToString())
	}
}

USkeletalMeshComponent* UArmorComponent::GetBodyArmorMesh()
{
	return Body;
}

USkeletalMeshComponent* UArmorComponent::GetArmsArmorMesh()
{
	return Arms;
}

USkeletalMeshComponent* UArmorComponent::GetShoulderArmorMesh()
{
	return Shoulders;
}

USkeletalMeshComponent* UArmorComponent::GetPantsMesh()
{
	return Pants;
}

USkeletalMeshComponent* UArmorComponent::GetHelmetMesh()
{
	return Helmet;
}

FName UArmorComponent::GetArmorName()
{
	return CurrentBreastplateName;
}

FName UArmorComponent::GetPantsName()
{
	return CurrentPantsName;
}

FName UArmorComponent::GetHelmetsName()
{
	return CurrentHelmetsName;
}

