// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterBase.h"
#include "PlayerSaveData.h"
#include "Playable.h"
#include "ObjectType.h"
#include "WeaponType.h"
#include "InputType.h"
#include "EternalGrace_ProtoCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UArmorComponent;
class UInventoryComponent;
class UWeaponComponent;
class AWeaponBase;
class UEG_AnimInstance;
class UInputBufferComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AEternalGrace_ProtoCharacter : public ACharacterBase, public IPlayable
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NormalAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NormalOffhandAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta =(AllowPrivateAccess))
	UWeaponComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UEG_AnimInstance* EGAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputBufferComponent* InputBufferingComponent;



public:
	AEternalGrace_ProtoCharacter();
	

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	//Saveable Implementations
	virtual void SaveData_Implementation()override;
	virtual void LoadData_Implementation()override;
	//Playable Implementations
	virtual void Interact_Implementation()override;
	virtual void ShowInteractUI_Implementation(AActor* ActorToInteractWith)override;
	virtual void HideInteractUI_Implementation()override;
	//Attackable Implementations
	virtual UCapsuleComponent* GetHitBox_Implementation()override;
	virtual AWeaponBase* GetWeapon_Implementation()override;
	virtual AWeaponBase* GetOffhandWeapon_Implementation()override;
	//Damageable Implementations
	virtual void GetDamage_Implementation(AActor* Attacker, float DamageValue, FVector ImpactPoint)override;
	virtual UAudioComponent* GetHitSoundComponent_Implementation()override;
	virtual UNiagaraSystem* GetHitEffectSystem_Implementation()override;
	//FootStep Sound
	virtual void PlayFootStepSound(FName FootSocket)override;


	void CancelGuard()override;
	void Jump()override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiplayer", meta = (AllowPrivateAccess))
	int32 PlayerIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame", meta = (AllowPrivateAccess))
	FName ObjectID;

	UPROPERTY()
	FPlayerSaveData SaveDataInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess))
	UArmorComponent* UpperArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess))
	UAnimMontage* DodgeMontage;


	UFUNCTION()
	void ShowInventory();

	UFUNCTION()
	void NormalAttack();
	UFUNCTION()
	void PerformDodgeAttack();
	UFUNCTION()
	void PerformOffhandAction();
	UFUNCTION()
	void TriggerCurrentInteractable();

	UFUNCTION()
	void Sprint();
	UFUNCTION()
	void CancelSprint();

	UFUNCTION()
	void Dodge();

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess))
	UInventoryComponent* PlayerInventory;
	


protected:
	virtual void BeginPlay()override;
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	void ChangeUpperArmor(FName ArmortID);
	UFUNCTION(BlueprintCallable)
	void ChangePants(FName PantsID);
	UFUNCTION(BlueprintCallable)
	void ChangeHelmet(FName HelmetID);

	UFUNCTION()
	UArmorComponent* GetArmorComponent();

	UFUNCTION()
	UInventoryComponent* GetInventory();

	UFUNCTION()
	void ObtainArmor(FName ArmorName, EObjectType ItemCategory);

	//While Object ID is an Identifier for the Save Game, the PlayerIndex holds track to wether its Player 1 or 2
	UFUNCTION()
	void SetPlayerIndex(int AssigendPlayerIndex);

	UFUNCTION()
	void EquipWeapon(TSubclassOf<AWeaponBase> WeaponToEquip);

	UFUNCTION()
	void EquipOffhandWeapon(TSubclassOf<AWeaponBase> WeaponToEquip);

	UFUNCTION(BlueprintCallable)
	void FireBufferedInput(EInputType BufferedInput);

	UFUNCTION()
	UInputBufferComponent* GetInputBufferComponent();
};

