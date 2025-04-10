// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EternalGrace_ProtoCharacter.h"
#include "EG_PlayerController.generated.h"

/**
 * 
 */
class UInventory;
class UButton;
class UInventorySlot;
class UObtainWidget;
class UBlendingWidget;
class UItemObtainContainerWidget;
UCLASS()
class ETERNALGRACE_PROTO_API AEG_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	AEG_PlayerController();
	void BeginPlay()override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess))
	TSubclassOf<UInventory> InventoryClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> InventorySlotClass; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess))
	TSubclassOf<UObtainWidget> ObtainWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess))
	TSubclassOf<UItemObtainContainerWidget> ObtainedItemsContainerClass;

	UPROPERTY()
	UItemObtainContainerWidget* ObtainedItemsContainer;

	UPROPERTY()
	UInventory* CurrentInventory;
	UPROPERTY()
	TArray<UInventorySlot*> CurrentInventorySlot;

	UPROPERTY()
	UUserWidget* InventoryObjectButtonWidget;

	UPROPERTY()
	AEternalGrace_ProtoCharacter* OwningCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> InteractInfoWidgetClass;
	UPROPERTY()
	UUserWidget* InteractInfoWidget;

	//DEATH HANDLING
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Interface", meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> YouDiedScreenClass;
	UPROPERTY()
	UBlendingWidget* YouDiedWidget;



public:
	UFUNCTION()
	void ShowYouDiedScreen();
	UFUNCTION()
	void HideYouDiedScreen();
	UFUNCTION()
	void HandlePlayerDeath(UEternalGrace_GameInstance* CurrentGameInstance);

	UFUNCTION()
	void ShowInventory();

	UFUNCTION()
	void ShowInteractInfoWidget();//TAKE ENUM AS INPUT
	UFUNCTION()
	void HideInteractInfoWidget();//TAKE ENUM AS INPUT

	UFUNCTION()
	AEternalGrace_ProtoCharacter* GetOwningCharacter();
	UFUNCTION()
	void SetOwningCharacterVariable(AEternalGrace_ProtoCharacter* SetCharacter);

	UFUNCTION()
	void ShowObtainWidget(FName ObjectID, TSoftObjectPtr<UTexture2D> ItemImage);

	UFUNCTION()
	void ReturnToMainMenu();
};
