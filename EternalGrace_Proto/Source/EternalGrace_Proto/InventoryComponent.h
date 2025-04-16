// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectType.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

struct FArmor;
struct FPants;
struct FHelmet;
struct FKeyItem;
class AWeaponBase;
class AEG_PlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GlobalData", meta = (AllowPrivateAccess))
	UDataTable* GlobalArmorInventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GlobalData", meta = (AllowPrivateAccess))
	UDataTable* GlobalPantsInventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GlobalData", meta = (AllowPrivateAccess))
	UDataTable* GlobalHelmetInventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GlobalData", meta = (AllowPrivateAccess))
	UDataTable* GlobalKeyItemInventory;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LocalData", meta =(AllowPrivateAccess))
	TMap<FName, int> ArmorInventoryMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LocalData", meta = (AllowPrivateAccess))
	TMap<FName, int> PantsInventoryMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LocalData", meta = (AllowPrivateAccess))
	TMap<FName, int> HelmetInventoryMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LocalData", meta = (AllowPrivateAccess))
	TMap<FName, int> KeyItemInventoryMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LocalData", meta = (AllowPrivateAccess))
	TArray<TSubclassOf<AWeaponBase>> WeaponInventory;

	UPROPERTY()
	AEG_PlayerController* OwningController;


	//This function is not used because I am to lazy to also make a setter. And since the component is already only accessible through a Getter, I think having the maps public should be fine.
	TMap<FName, int>* GetInventoryOfType(EObjectType ItemCategory);

	UFUNCTION()
	void AddArmorToInventory(FArmor ArmorToAdd);
	UFUNCTION()
	void AddPantsToInventory(FPants PantsToAdd);
	UFUNCTION()
	void AddHelmetToInventory(FHelmet HelmetToAdd);
	UFUNCTION()
	void AddKeyItemToInventory(FKeyItem KeyItemToAdd);

	UFUNCTION()
	void AddWeaponToInventory(TSubclassOf<AWeaponBase> WeaponToAdd);
	UFUNCTION()
	void TryToObtainItem(FName ObjectName, EObjectType ItemCategory);

	UFUNCTION()
	void ShowObtainItemWidget(FName ObjectID, TSoftObjectPtr<UTexture2D> ThumpNail);

		
};
