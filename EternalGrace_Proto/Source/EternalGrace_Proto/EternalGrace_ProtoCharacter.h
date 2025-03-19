// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterBase.h"
#include "PlayerSaveData.h"
#include "Playable.h"
#include "ObjectType.h"
#include "EternalGrace_ProtoCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UArmorComponent;
class UInventoryComponent;
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
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MenuAction;



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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiplayer", meta = (AllowPrivateAccess))
	int32 PlayerIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame", meta = (AllowPrivateAccess))
	FName ObjectID;

	UPROPERTY()
	FPlayerSaveData SaveDataInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess))
	UArmorComponent* UpperArmor;

	UFUNCTION(CallInEditor)
	void ShowInventory();

	UFUNCTION()
	void TriggerCurrentInteractable();

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

	UFUNCTION(CallInEditor, Category = "Equipment Test")
	void ChangeUpperArmor();
	UFUNCTION(CallInEditor, Category = "Equipment Test")
	void ChangePants();
	UFUNCTION()
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
};

