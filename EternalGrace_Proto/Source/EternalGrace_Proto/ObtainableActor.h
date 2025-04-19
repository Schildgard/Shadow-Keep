// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "WeaponBase.h"
#include "TreasureChestSaveData.h"
#include "Saveable.h"
#include "ObtainableActor.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALGRACE_PROTO_API AObtainableActor : public AInteractableBase, public ISaveable
{
	GENERATED_BODY()

	AObtainableActor();
	/*This class mostly works exactly like the treasure Chest. However it has a Static Mesh, no animation logic and is not destructable. Also it despawns on reload, instead of beeing empty.*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess))
	TArray<FName> ContainedArmory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess))
	TArray<FName> ContainedLegArmor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess))
	TArray<FName> ContainedHelmets;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess))
	TArray<FName> ContainedUseables;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess))
	TArray<FName> ContainedKeyItems;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess))
	TArray<TSubclassOf<AWeaponBase>> ContainedWeapons;

	UPROPERTY()
	AEternalGrace_ProtoCharacter* Interactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save", meta = (AllowPrivateAccess))
	FName ObjectID;

	UPROPERTY()
	FTreasureChestSaveData SaveDataInfo;

public:
	UFUNCTION()
	void PickUp();

	virtual void GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)override;
	virtual void SaveData_Implementation()override;
	virtual void LoadData_Implementation()override;
	
};
