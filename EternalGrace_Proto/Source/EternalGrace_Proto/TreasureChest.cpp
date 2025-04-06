// Fill out your copyright notice in the Description page of Project Settings.


#include "TreasureChest.h"
#include "EternalGrace_ProtoCharacter.h"
#include "InventoryComponent.h"
#include "EternalGrace_GameInstance.h"
#include "EternalGrace_SaveGame.h"
#include "WeaponBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ATreasureChest::ATreasureChest()
{
	GeometryComponent = CreateDefaultSubobject<UGeometryCollectionComponent>("GeometryComponent");
	GeometryComponent->SetupAttachment(RootComponent);
	GeometryComponent->SetVisibility(false);
	GeometryComponent->SetAutoActivate(false);
	GeometryComponent->SetSimulatePhysics(false);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(Mesh);
}

void ATreasureChest::GetAllTreasure()
{
	if (!Interactor)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Interactor assigned to this chest! %s"), *GetFName().ToString());
		return;
	}

	if (!SaveDataInfo.bCanBeInteractedWith)
	{
		UE_LOG(LogTemp, Warning, TEXT("This Chest %s has already been opened"), *GetFName().ToString())
			return;
	}
	Interactor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	// Interactor checks in Armor Component if this Armor generally exists.
	// If so, The Inventory Component checks if its already in player inventory and adds it accordingly.
	for (FName ArmorName : ContainedArmory)
	{
		Interactor->ObtainArmor(ArmorName, EObjectType::BodyEquipment);
	}
	ContainedArmory.Empty();

	for (FName PantsName : ContainedLegArmor)
	{
		Interactor->ObtainArmor(PantsName, EObjectType::LegEquipment);
	}
	ContainedLegArmor.Empty();

	for (FName HelmetName : ContainedHelmets)
	{
		Interactor->ObtainArmor(HelmetName, EObjectType::HeadEquipment);
	}
	ContainedHelmets.Empty();

	for (TSubclassOf<AWeaponBase> Weapon : ContainedWeapons)
	{
		Interactor->GetInventory()->AddWeaponToInventory(Weapon);
	}
	ContainedWeapons.Empty();
	Execute_SaveData(this);

	Interactor = nullptr;
}

void ATreasureChest::TriggerChest(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (Mesh->AnimationData.AnimToPlay && Interactor)
	{
		Mesh->Play(false);
		if (OpenSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), OpenSound, GetActorLocation());

		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Animation assigned or is already open"), *GetFName().ToString());
	}
}

UGeometryCollectionComponent* ATreasureChest::GetGeometryComp()
{
	return GeometryComponent;
}

void ATreasureChest::GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)
{
	if (!bCanbeActivated)
	{
		UE_LOG(LogTemp, Error, TEXT("Trease can not be Activated anymore"));
		return;
	}
	bCanbeActivated = false;
	Interactor = InteractingPlayer;
	Interactor->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "s_Interact");
	UAnimInstance* PlayerAnimationInstance = Interactor->GetMesh()->GetAnimInstance();
	if (PlayerAnimationInstance)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Interactor->GetActorLocation(), GetActorLocation());
		FRotator DesiredActorRotation = FRotator(0, LookAtRotation.Yaw, 0);
		Interactor->SetActorRotation(DesiredActorRotation, ETeleportType::ResetPhysics);
		PlayerAnimationInstance->Montage_Play(ActorInteractMontage);

		PlayerAnimationInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ATreasureChest::TriggerChest);
	}
}

void ATreasureChest::SaveData_Implementation()
{
	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetGameInstance());
	if (CurrentGameInstance)
	{
		UEternalGrace_SaveGame* CurrentSavegame = CurrentGameInstance->GetCurrentSaveGame();
		if (CurrentSavegame)
		{
			//Update SaveData Struct
			SaveDataInfo.bCanBeInteractedWith = bCanbeActivated;
			//Actually SaveData to SaveGame
			CurrentSavegame->SaveTreasureChestData(ObjectID, SaveDataInfo);
		}
		CurrentGameInstance->SavePlayerData();
	}
}

void ATreasureChest::LoadData_Implementation()
{
	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetGameInstance());
	if (CurrentGameInstance)
	{
		UEternalGrace_SaveGame* CurrentSavegame = CurrentGameInstance->GetCurrentSaveGame();
		{
			if (CurrentSavegame)
			{
				FTreasureChestSaveData* SavedData = CurrentSavegame->LoadTreasureChestData(ObjectID);
				//Try to load Data
				if (SavedData)
				{
					SaveDataInfo = *SavedData;
					if (!SaveDataInfo.bCanBeInteractedWith)
					{
						//When Chest has already been opened, trigger open anim
						Mesh->Play(false);
					}
				}
			}
		}
	}
}

void ATreasureChest::GetDamage_Implementation(AActor* Attacker, float DamageValue, float PoiseDamage, EAttackDirection Direction)
{
	if (bCanbeActivated) return;
	//GetDamage Enables the ChaosDestruction To Function
	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GeometryComponent->SetVisibility(true);
	GeometryComponent->SetSimulatePhysics(true);

//	GeometryComponent->ApplyExternalStrain(0, ImpactPoint, 100.0f, 1, 1.0f, 50000.0f);
	GeometryComponent->ApplyLinearVelocity(0, Attacker->GetActorForwardVector() * DamageValue);
	GeometryComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	

	AudioComponent->Play();
}

UAudioComponent* ATreasureChest::GetHitSoundComponent_Implementation()
{
	return AudioComponent;
}

UNiagaraSystem* ATreasureChest::GetHitEffectSystem_Implementation()
{
	return nullptr;
}
