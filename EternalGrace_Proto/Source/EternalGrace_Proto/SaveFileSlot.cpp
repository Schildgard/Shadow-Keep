// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveFileSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "EternalGrace_GameInstance.h"
#include "TitleScreenOptions.h"
#include "ButtonWidget.h"

void USaveFileSlot::NativeConstruct()
{
	LoadFileButton->OnClicked.AddDynamic(this, &USaveFileSlot::LoadGame);
	DeleteFileButton->OnClicked.AddDynamic(this, &USaveFileSlot::DeleteFile);
	SetDesiredFocusWidget(LoadFileButton);
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


	//Check if this Widget is at first place of SaveGamelist (-1 because the return button also is a child, but should not be considered in this count)
	int NumberOfSaveFiles = GetParent()->GetChildrenCount()-1;
	UE_LOG(LogTemp, Warning, TEXT("%i Children"), NumberOfSaveFiles);
	if (NumberOfSaveFiles >= 2)
	{
		if (GetParent()->GetChildAt(0) == this)
		{
			//If so, the focus is set to next object in the list
			GetParent()->GetChildAt(1)->SetFocus();

		}
		else
		{
			//If not, set focus on first object in the list
			GetParent()->GetChildAt(0)->SetFocus();
		}
	}
	else if (Instigator) //If there is only 1 save file, set the focus to return button
	{
		Instigator->GetCloseSaveFileButton()->SetFocus();
	}
	CurrentGameInstance->DeleteFile(SlotID);
	RemoveFromParent();

}

void USaveFileSlot::SetInstigator(UTitleScreenOptions* TitleScreen)
{
	Instigator = TitleScreen;
}

