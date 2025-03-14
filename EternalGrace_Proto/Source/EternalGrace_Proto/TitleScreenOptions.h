// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleScreenOptions.generated.h"

/**
 * 
 */
class UButton;
class UVerticalBox;
class USaveFileSlot;
class UEternalGrace_GameInstance;
UCLASS()
class ETERNALGRACE_PROTO_API UTitleScreenOptions : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct()override;

	UPROPERTY(meta =(BindWidget), Transient)
	UButton* StartNewGameButton;
	UPROPERTY(meta = (BindWidget), Transient)
	UButton* GoBackButton;
	UPROPERTY(meta = (BindWidget), Transient)
	UButton* LoadGameButton;
	UPROPERTY(meta = (BindWidget), Transient)
	UButton* SinglePlayerModeSelectionButton;
	UPROPERTY(meta = (BindWidget), Transient)
	UButton* MultiPlayerModeSelectionButton;

	UPROPERTY(Transient)
	bool bAreLoadButtonsVisible;

	UPROPERTY(meta = (BindWidget), Transient)
	UVerticalBox* ButtonAllignmentBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (AllowPrivateAccess))
	TSubclassOf<USaveFileSlot> SaveFileSlotWidgetClass;

	UPROPERTY()
	UEternalGrace_GameInstance* CurrentGameInstance;
	
	UFUNCTION()
	void StartNewGame();

	UFUNCTION()
	void ShowAviableSaveGames();
	UFUNCTION()
	void SetMultiPlayerMode();
	UFUNCTION()
	void ShowPlayerModeOptions();
	UFUNCTION()
	void ReturnToSaveGameSelection();
};
