// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleScreenOptions.h"
#include "EternalGrace_GameInstance.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "SaveFileSlot.h"
#include "ButtonWidget.h"
#include "Kismet/GameplayStatics.h"

void UTitleScreenOptions::NativeConstruct()
{
	Super::NativeConstruct();
	bAreLoadButtonsVisible = false;
	CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetGameInstance());
	if (!CurrentGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Warning! GameInstance Cast by TitleScreen Failed!"));
	}
	StartNewGameButton->GetWidgetButton()->OnClicked.AddDynamic(this, &UTitleScreenOptions::ShowPlayerModeOptions);
	StartNewGameButton->SetKeyboardFocus();
	GoBackButton->GetWidgetButton()->OnClicked.AddDynamic(this, &UTitleScreenOptions::ReturnToSaveGameSelection);
	LoadGameButton->GetWidgetButton()->OnClicked.AddDynamic(this, &UTitleScreenOptions::ShowAviableSaveGames);
	SinglePlayerModeSelectionButton->GetWidgetButton()->OnClicked.AddDynamic(this, &UTitleScreenOptions::ShowClassSelection); //that was previously on StartNewGame
	MultiPlayerModeSelectionButton->GetWidgetButton()->OnClicked.AddDynamic(this, &UTitleScreenOptions::SetMultiPlayerMode);
	CloseSaveFileButton->GetWidgetButton()->OnClicked.AddDynamic(this, &UTitleScreenOptions::ShowAviableSaveGames); //If Savegame list is already open, it closes
	//ClassSelectionWidget->GetWidgetButton()->OnClicked.AddDynamic(this, &UTitleScreenOptions::SetPlayerStartingClass);

}

void UTitleScreenOptions::StartNewGame()
{
	int SlotNumber = CurrentGameInstance->GetAllSaveGames().Num() + 1;
	FName NewSlotName;
	bool bIsMultiplayerActivated = CurrentGameInstance->GetTwoPlayerMode();
	if (bIsMultiplayerActivated)
	{
		NewSlotName = FName(TEXT("Multiplayer Session 0"), SlotNumber);
	}
	else
	{
		NewSlotName = FName(TEXT("Singleplayer Session 0"), SlotNumber);
	}

	CurrentGameInstance->SetActiveSaveGameSlot(NewSlotName);
	CurrentGameInstance->StartGame();
}

void UTitleScreenOptions::ShowAviableSaveGames()
{
	if (bAreLoadButtonsVisible)
	{
		ButtonAllignmentBox->ClearChildren();
		bAreLoadButtonsVisible = false;
		StartNewGameButton->SetFocus();
		return;
	}
	if (SaveFileSlotWidgetClass)
	{
		USaveFileSlot* SaveFile;
		TArray<FName> SaveGameList = CurrentGameInstance->GetAllSaveGames();
		int i = 0;
		for (FName SaveGameID : SaveGameList)
		{
			SaveFile = CreateWidget<USaveFileSlot>(this, SaveFileSlotWidgetClass);
			if (SaveFile)
			{
				ButtonAllignmentBox->AddChildToVerticalBox(SaveFile);
				SaveFile->SetSlotID(SaveGameID);
				SaveFile->SetInstigator(this);
				if (i == 0)
				{
					SaveFile->GetLoadButton()->SetFocus();
				}
			}
			i++;
		}
		if (SaveGameList.Num() >= 1)
		{
			ButtonAllignmentBox->AddChildToVerticalBox(CloseSaveFileButton);
			CloseSaveFileButton->SetVisibility(ESlateVisibility::Visible);
			bAreLoadButtonsVisible = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveFileSlotWidgetClass was not set"))
	}
}

void UTitleScreenOptions::SetMultiPlayerMode()
{
	CurrentGameInstance->SetMultiplayer(true);
	APlayerController* Player1Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APlayerController* Player2Controller = UGameplayStatics::CreatePlayer(GetWorld(), 1);

	if (ClassSelectionWidgetClass)
	{
		UUserWidget* ClassSelect = CreateWidget<UUserWidget>(Player1Controller, ClassSelectionWidgetClass);
		if (ClassSelect)
		{
			ClassSelect->AddToPlayerScreen();
			UE_LOG(LogTemp, Warning, TEXT("ClassSelect added to viewport. Owned by Controller: %s"), *Player1Controller->GetFName().ToString());
		}
		UUserWidget* ClassSelect2 = CreateWidget<UUserWidget>(Player2Controller, ClassSelectionWidgetClass);
		if (ClassSelect2)
		{
			ClassSelect2->AddToPlayerScreen();
			UE_LOG(LogTemp, Warning, TEXT("ClassSelect added to viewport. Owned by Controller: %s"), *Player2Controller->GetFName().ToString());
		}

	}

	SinglePlayerModeSelectionButton->SetVisibility(ESlateVisibility::Collapsed);
	MultiPlayerModeSelectionButton->SetVisibility(ESlateVisibility::Collapsed);
	GoBackButton->SetVisibility(ESlateVisibility::Collapsed);
	//StartNewGame();
}

void UTitleScreenOptions::ShowPlayerModeOptions()
{
	StartNewGameButton->SetVisibility(ESlateVisibility::Collapsed);
	LoadGameButton->SetVisibility(ESlateVisibility::Collapsed);
	SinglePlayerModeSelectionButton->SetVisibility(ESlateVisibility::Visible);
	SinglePlayerModeSelectionButton->SetKeyboardFocus();
	MultiPlayerModeSelectionButton->SetVisibility(ESlateVisibility::Visible);
	GoBackButton->SetVisibility(ESlateVisibility::Visible);
}

void UTitleScreenOptions::ReturnToSaveGameSelection()
{
	StartNewGameButton->SetVisibility(ESlateVisibility::Visible);
	StartNewGameButton->SetFocus();
	LoadGameButton->SetVisibility(ESlateVisibility::Visible);
	GoBackButton->SetVisibility(ESlateVisibility::Collapsed);
	SinglePlayerModeSelectionButton->SetVisibility(ESlateVisibility::Collapsed);
	MultiPlayerModeSelectionButton->SetVisibility(ESlateVisibility::Collapsed);

}

void UTitleScreenOptions::PlayOnSelectedSound()
{

}

void UTitleScreenOptions::ShowClassSelection()
{
	
	if(ClassSelectionWidgetClass)
	{
		UUserWidget* ClassSelect = CreateWidget<UUserWidget>(GetOwningPlayer(), ClassSelectionWidgetClass);
		if(ClassSelect)
		{
			ClassSelect->AddToPlayerScreen();
			UE_LOG(LogTemp, Warning, TEXT("ClassSelect added to viewport. Owned by Controller: %s"), *GetOwningPlayer()->GetFName().ToString());
		}
	}
//	if (ClassSelectionWidget->GetVisibility() != ESlateVisibility::Visible)
//	{
		SinglePlayerModeSelectionButton->SetVisibility(ESlateVisibility::Collapsed);
		MultiPlayerModeSelectionButton->SetVisibility(ESlateVisibility::Collapsed);
		GoBackButton->SetVisibility(ESlateVisibility::Collapsed);
//
//		ClassSelectionWidget->SetVisibility(ESlateVisibility::Visible);
//	}
//	else
//	{
//		ClassSelectionWidget->SetVisibility(ESlateVisibility::Collapsed);
//		ShowPlayerModeOptions();
//	}
}

void UTitleScreenOptions::SetPlayerStartingClass()
{
	UE_LOG(LogTemp, Warning, TEXT("Chose Player Class"))
}

UButtonWidget* UTitleScreenOptions::GetCloseSaveFileButton()
{
	return CloseSaveFileButton;
}
