// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "EternalGrace_ProtoCharacter.h"
#include "ObjectType.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALGRACE_PROTO_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	void NativeConstruct()override;

	UPROPERTY(meta = (BindWidget))
	UButton* SlotButton;

	UPROPERTY(meta =(BindWidget))
	UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value", meta = (AllowPrivateAccess))
	FName ItemName;

	UPROPERTY()
	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY()
	AEternalGrace_ProtoCharacter* OwningCharacter;

	UPROPERTY()
	EObjectType ItemCategory;

public:
	UFUNCTION()
	void UseInventorySlot();
	UFUNCTION()
	UButton* GetSlotButton();
	UFUNCTION()
	UImage* GetImage();
	UFUNCTION()
	void SetObjectType(EObjectType NewItemCategory);
	UFUNCTION()
	void SetImage(UTexture2D* ImageToSet);
	UFUNCTION()
	void SetItemName(FName Name);
	UFUNCTION()
	void SetWeapon(TSubclassOf<AWeaponBase> WeaponToRepresent);
};
