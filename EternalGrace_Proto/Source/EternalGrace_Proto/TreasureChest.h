// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor_Dynamic.h"
#include "ObjectType.h"
#include "Saveable.h"
#include "Damageable.h"
#include "TreasureChestSaveData.h"
#include "Chaos/ChaosSolverActor.h"
#include "Chaos/ChaosEngineInterface.h"
#include "TreasureChest.generated.h"

/**
 * 
 */
class AEternalGrace_ProtoCharacter;
class UEternalGrace_SaveGame;
class AWeaponBase;
class UGeometryCollectionComponent;
UCLASS()
class ETERNALGRACE_PROTO_API ATreasureChest : public AInteractableActor_Dynamic, public ISaveable, public IDamageable
{
	GENERATED_BODY()

protected:
	ATreasureChest();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics", meta = (AllowPrivateAccess))
	UGeometryCollectionComponent* GeometryComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess))
	USoundBase* OpenSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess))
	USoundBase* DestructionSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess))
	UAudioComponent* AudioComponent;
	

public:
	UFUNCTION()
	void GetAllTreasure();
	UFUNCTION()
	void TriggerChest(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	UGeometryCollectionComponent* GetGeometryComp();

	//Save Interface Implementations
	virtual void GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)override;
	virtual void SaveData_Implementation()override;
	virtual void LoadData_Implementation()override;
	//Damageable Interface Implementations
	virtual void GetDamage_Implementation(AActor* Attacker, float DamageValue, float PoiseDamage, EAttackDirection Direction)override;
	virtual UAudioComponent* GetHitSoundComponent_Implementation()override;
	virtual UNiagaraSystem* GetHitEffectSystem_Implementation()override;
	
};
