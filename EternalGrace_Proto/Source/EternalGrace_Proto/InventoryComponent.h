// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectType.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

struct FArmor;
struct FPants;
struct FHelmet;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GlobalData", meta = (AllowPrivateAccess))
	UDataTable* GlobalArmorInventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GlobalData", meta = (AllowPrivateAccess))
	UDataTable* GlobalPantsInventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GlobalData", meta = (AllowPrivateAccess))
	UDataTable* GlobalHelmetInventory;


	UPROPERTY()
	UDataTable* ArmorInventory;
	UPROPERTY()
	UDataTable* PantsInventory;
	UPROPERTY()
	UDataTable* HelmetInventory;

public:	

	UFUNCTION()
	UDataTable* GetArmorInventory();
	UFUNCTION()
	UDataTable* GetPantsInventory();
	UFUNCTION()
	UDataTable* GetHelmetInventory();

	UFUNCTION()
	void AddArmorToInventory(FArmor ArmorToAdd);
	UFUNCTION()
	void AddPantsToInventory(FPants PantsToAdd);
	UFUNCTION()
	void AddHelmetToInventory(FHelmet HelmetToAdd);

	UFUNCTION()
	void TryToObtainItem(FName ObjectName, EObjectType ItemCategory);

		
};
