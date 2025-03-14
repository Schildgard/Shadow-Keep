// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleScreenOptions.h"
#include "EternalGrace_GameInstance.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "SaveFileSlot.h"

void UTitleScreenOptions::NativeConstruct()
{
	Super::NativeConstruct();
	bAreLoadButtonsVisible = false;
	CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetGameInstance());
	if (!CurrentGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Warning! GameInstance Cast by TitleScreen Failed!"));
	}
	StartNewGameButton->OnClicked.AddDynamic(this, &UTitleScreenOptions::ShowPlayerModeOptions);
	GoBackButton->OnClicked.AddDynamic(this, &UTitleScreenOptions::ReturnToSaveGameSelection);
	LoadGameButton->OnClicked.AddDynamic(this, &UTitleScreenOptions::ShowAviableSaveGames);
	SinglePlayerModeSelectionButton->OnClicked.AddDynamic(this, &UTitleScreenOptions::StartNewGame);
	MultiPlayerModeSelectionButton->OnClicked.AddDynamic(this, &UTitleScreenOptions::SetMultiPlayerMode);
}

void UTitleScreenOptions::StartNewGame()
{
	int SlotNumber = CurrentGameInstance->GetAllSaveGames().Num() + 1;
	FName NewSlotName = FName(TEXT("SaveFile %i"), SlotNumber);
	CurrentGameInstance->SetActiveSaveGameSlot(NewSlotName);
	CurrentGameInstance->StartGame();
}

void UTitleScreenOptions::ShowAviableSaveGames()
{
	if(bAreLoadButtonsVisible)
	{
		ButtonAllignmentBox->ClearChildren();
		bAreLoadButtonsVisible = false;
		return;
	}
	if (SaveFileSlotWidgetClass)
	{
		USaveFileSlot* SaveFile;
		TArray<FName> SaveGameList = CurrentGameInstance->GetAllSaveGames();
		for (FName SaveGameID : SaveGameList)
		{
			SaveFile = CreateWidget<USaveFileSlot>(this, SaveFileSlotWidgetClass);
			if (SaveFile)
			{
				ButtonAllignmentBox->AddChildToVerticalBox(SaveFile);
				SaveFile->SetSlotID(SaveGameID);
			}
		}
		bAreLoadButtonsVisible = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveFileSlotWidgetClass was not set"))
	}
}

void UTitleScreenOptions::SetMultiPlayerMode()
{
	CurrentGameInstance->SetMultiplayer(true);
	StartNewGame();
}

void UTitleScreenOptions::ShowPlayerModeOptions()
{
	StartNewGameButton->SetVisibility(ESlateVisibility::Collapsed);
	LoadGameButton->SetVisibility(ESlateVisibility::Collapsed);
	SinglePlayerModeSelectionButton->SetVisibility(ESlateVisibility::Visible);
	MultiPlayerModeSelectionButton->SetVisibility(ESlateVisibility::Visible);
	GoBackButton->SetVisibility(ESlateVisibility::Visible);
}

void UTitleScreenOptions::ReturnToSaveGameSelection()
{
	StartNewGameButton->SetVisibility(ESlateVisibility::Visible);
	LoadGameButton->SetVisibility(ESlateVisibility::Visible);
	GoBackButton->SetVisibility(ESlateVisibility::Collapsed);
	SinglePlayerModeSelectionButton->SetVisibility(ESlateVisibility::Collapsed);
	MultiPlayerModeSelectionButton->SetVisibility(ESlateVisibility::Collapsed);
}
