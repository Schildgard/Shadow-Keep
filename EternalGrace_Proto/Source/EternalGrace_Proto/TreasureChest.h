// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor_Dynamic.h"
#include "ObjectType.h"
#include "Saveable.h"
#include "TreasureChestSaveData.h"
#include "TreasureChest.generated.h"

/**
 * 
 */
class AEternalGrace_ProtoCharacter;
class UEternalGrace_SaveGame;
class AWeaponBase;
UCLASS()
class ETERNALGRACE_PROTO_API ATreasureChest : public AInteractableActor_Dynamic, public ISaveable
{
	GENERATED_BODY()

protected:
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess))
	UAnimMontage* ActorInteractMontage;

public:
	UFUNCTION()
	void GetAllTreasure();
	UFUNCTION()
	void TriggerChest(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	virtual void GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)override;
	virtual void SaveData_Implementation()override;
	virtual void LoadData_Implementation()override;
	
};
