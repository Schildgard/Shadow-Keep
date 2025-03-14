// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveFileSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "EternalGrace_GameInstance.h"

void USaveFileSlot::NativeConstruct()
{
	LoadFileButton->OnClicked.AddDynamic(this, &USaveFileSlot::LoadGame);
	DeleteFileButton->OnClicked.AddDynamic(this, &USaveFileSlot::DeleteFile);
}

UButton* USaveFileSlot::GetLoadButton()
{
	return LoadFileButton;
}

void USaveFileSlot::SetSlotID(FName FileSlotID)
{
	SlotID = FileSlotID;
	ButtonText->SetText(FText::FromName(SlotID));
}

void USaveFileSlot::LoadGame()
{
	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetGameInstance());
	if (!CurrentGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance Cast by LoadingButton Failed!"));
		return;
	}
	CurrentGameInstance->SetActiveSaveGameSlot(SlotID);
	CurrentGameInstance->ResumeGame();
}

void USaveFileSlot::DeleteFile()
{
	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetGameInstance());
	if (!CurrentGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance Cast by LoadingButton Failed!"));
		return;
	}
	CurrentGameInstance->DeleteFile(SlotID);
	RemoveFromParent();
}

